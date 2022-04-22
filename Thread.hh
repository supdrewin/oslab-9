/// Thread.hh - This file is a part of oslab-9 (not project)
/// Copyright © 2022  Supdrewin <https://github.com/supdrewin/oslib-9>
///
/// This program is free software: you can redistribute it and/or modify it
/// under the terms of the GNU General Public License as published by the Free
/// Software Foundation, either version 3 of the License, or (at your option)
/// any later version.
///
/// This program is distributed in the hope that it will be useful, but WITHOUT
/// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
/// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
/// more details.
///
/// You should have received a copy of the GNU General Public License along with
/// this program.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include <thread>

#if defined(__cpp_lib_jthread)

using StopToken = std::stop_token;
using Thread = std::jthread;

#else

#include <memory>

struct StopSource {
    using StateType = std::shared_ptr<bool>;

    std::shared_ptr<bool> state;

    StopSource(bool* state = nullptr)
        : state(state)
    {
    }

    auto swap(StopSource& other) -> void
    {
        other.state.swap(this->state);
    }

    auto operator()(bool state) -> void
    {
        *this->state.get() = state;
    }
};

class StopToken {
public:
    explicit StopToken(StopSource& stop_src)
        : state(stop_src.state.get())
    {
    }

    auto stop_requested() -> bool
    {
        return *this->state.get();
    }

private:
    StopSource::StateType state;
};

class Thread {
public:
    Thread() noexcept
        : stop_src()
        , thread()
    {
    }

    auto get_id() const
        -> std::thread::id
    {
        return this->thread.get_id();
    }

    template <typename Fn, typename... Args>
    explicit Thread(Fn&& task, Args&&... args)
        : stop_src(new bool(false))
        , thread()
    {
        std::thread thread(task, StopToken(this->stop_src), args...);
        this->thread.swap(thread);
    }

    Thread(Thread const&) = delete;
    Thread(Thread&&) noexcept = default;

    ~Thread()
    {
        if (this->joinable()) {
            this->request_stop();
            this->join();
        }
    }

    Thread& operator=(Thread const&) = delete;

    auto request_stop() -> void
    {
        this->stop_src(true);
    }

    auto detach() -> void
    {
        (**this).detach();
    }

    auto joinable() -> bool
    {
        return (**this).joinable();
    }

    auto join() -> void
    {
        (**this).join();
    }

    auto swap(Thread& other) -> void
    {
        other.stop_src.swap(this->stop_src);
        (*other).swap(**this);
    }

    auto operator*() -> std::thread&
    {
        return this->thread;
    }

    friend auto
    swap(Thread& a, Thread& b)
        -> void
    {
        a.swap(b);
    }

private:
    StopSource stop_src;
    std::thread thread;
};

#endif

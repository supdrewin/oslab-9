/// JThread.hh - This file is a part of oslab-9 (not project)
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

#include "StopSource.hh"
#include "StopToken.hh"

#define HAS_JTHREAD_CLASS
#include "Thread.hh"

class JThread {
public:
    using id = Thread::id;
    using native_handle_type = Thread::native_handle_type;

    JThread() noexcept
        : stop_source()
        , thread()
    {
    }

    template <typename Fn, typename... Args>
    explicit JThread(Fn&& task, Args&&... args)
        : stop_source(new bool(false))
        , thread()
    {
        std::thread thread(task, StopToken(this->stop_source), args...);
        this->thread.swap(thread);
    }

    ~JThread()
    {
        if (this->joinable()) {
            this->request_stop();
            this->join();
        }
    }

    JThread(JThread const&) = delete;
    JThread(JThread&&) noexcept = default;

    JThread& operator=(const JThread&) = delete;
    JThread& operator=(JThread&&) noexcept = default;

    auto swap(JThread& other) noexcept -> void
    {
        other.stop_source.swap(this->stop_source);
        other.thread.swap(this->thread);
    }

    [[nodiscard]] auto joinable() -> bool
    {
        return this->thread.joinable();
    }

    auto join() -> void
    {
        this->thread.join();
    }

    auto detach() -> void
    {
        this->thread.detach();
    }

    [[nodiscard]] auto get_id() const noexcept
        -> id
    {
        return this->thread.get_id();
    }

    [[nodiscard]] auto native_handle()
        -> native_handle_type
    {
        return this->thread.native_handle();
    }

    [[nodiscard]] auto get_stop_source() noexcept
        -> StopSource
    {
        return this->stop_source;
    }

    [[nodiscard]] auto get_stop_token() const noexcept
        -> StopToken
    {
        return StopToken(this->stop_source);
    }

    auto request_stop() noexcept -> void
    {
        this->stop_source(true);
    }

    friend auto
    swap(JThread& lhs, JThread& rhs) noexcept -> void
    {
        lhs.swap(rhs);
    }

    [[nodiscard]] static auto
    hardware_concurrency() noexcept -> unsigned
    {
        return Thread::hardware_concurrency();
    }

private:
    StopSource stop_source;
    Thread thread;
};

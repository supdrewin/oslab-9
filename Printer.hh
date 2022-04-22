/// printer.hh - This file is a part of oslab-9 (not project)
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

#include <cstddef>
#include <mutex>

#include "public.hh"

class Printer {
public:
    using RefreshRateType = std::chrono::milliseconds;
    using ConstRefType = Printer const&;

    Printer(size_t refresh_rate)
        : mutex()
        , refresh_event()
        , call_back()
        , _refresh_rate(refresh_rate)
    {
    }

    Printer(ConstRefType) = delete;

    template <typename T>
    T operator=(ConstRefType) = delete;

    template <typename T>
    T* operator=(ConstRefType) = delete;

    auto refresh_rate() -> RefreshRateType
    {
        return this->_refresh_rate;
    }

    template <typename Fn, typename... Args>
    auto set_refresh_event(Fn&& refresh_event, Args&&... args)
        -> void
    {
        JThread re(refresh_event, args...);
        this->refresh_event.swap(re);
    }

    template <typename Fn, typename... Args>
    auto set_call_back(Fn&& call_back, Args&&... args)
        -> void
    {
        JThread cb(call_back, args...);
        this->call_back.swap(cb);
    }

    auto wait_to_stop() -> void
    {
        if (this->call_back.joinable()) {
            this->call_back.request_stop();
            this->call_back.join();
        }

        if (this->refresh_event.joinable()) {
            this->refresh_event.request_stop();
            this->refresh_event.join();
        }
    }

    template <typename Fn, typename... Args>
    auto process(Fn&& printer_task, Args&&... args)
        -> decltype(printer_task(args...))
#pragma mark - sync
    {
        Printer::Mutex lock(this->mutex);
        return printer_task(args...);
    }
#pragma mark - sync

private:
    struct Mutex {
        std::mutex& mutex;

        explicit Mutex(std::mutex& mutex)
            : mutex(mutex)
        {
            mutex.lock();
        }

        ~Mutex()
        {
            mutex.unlock();
        }
    };

    std::mutex mutex;

    JThread refresh_event;
    JThread call_back;

    RefreshRateType _refresh_rate;
};

#define HAS_PRINTER_CLASS

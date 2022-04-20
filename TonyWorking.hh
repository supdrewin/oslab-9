/// TonyWorking.hh - This file is a part of oslib-9 (not project)
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

class TonyWorking {
public:
    TonyWorking(std::mutex* mutex)
        : mutex(mutex)
    {
    }

    ~TonyWorking()
    {
        this->sleep();
    }

    auto operator()(size_t msec) -> void
    {
        using namespace std;
        this_thread::sleep_for(
            chrono::milliseconds(msec));
    }

private:
    auto sleep() -> void
    {
        this->mutex->unlock();
    }

    std::mutex* mutex;
};

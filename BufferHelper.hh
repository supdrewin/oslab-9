/// BufferHelper.hh - This file is a part of oslib-9 (not project)
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

#include <deque>
#include <functional>
#include <mutex>

class BufferHelper {
public:
    BufferHelper(std::deque<char>* deque,
        std::mutex* mutex)
        : deque(deque)
        , mutex(mutex)
    {
    }

    ~BufferHelper()
    {
        this->mutex->unlock();
    }

    auto read() -> char
    {
        return (**this).empty()
            ? '\0'
            : (**this).front();
    }

    auto take_if(std::function<bool(char)> cmp)
        -> char
    {
        auto c = this->read();

        if (cmp(c)) {
            (**this).pop_front();
            return c;
        }

        return '\0';
    }

    auto write(char c) -> void
    {
        (**this).push_back(c);
    }

    auto operator*() -> std::deque<char>&
    {
        return *this->deque;
    }

private:
    std::deque<char>* deque;
    std::mutex* mutex;
};

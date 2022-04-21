/// Buffer.hh - This file is a part of oslab-9 (not project)
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
#include <memory>
#include <mutex>

#include "BufferHelper.hh"

class Buffer {
public:
    Buffer()
        : deque()
        , mutex()
    {
    }

    auto lock() -> std::unique_ptr<BufferHelper>
    {
        this->mutex.lock();
        return std::unique_ptr<BufferHelper> {
            new BufferHelper(deque, mutex)
        };
    }

    auto try_lock() -> std::unique_ptr<BufferHelper>
    {
        return std::unique_ptr<BufferHelper> {
            this->mutex.try_lock()
                ? new BufferHelper(deque, mutex)
                : nullptr
        };
    }

private:
    std::deque<char> deque;
    std::mutex mutex;
};

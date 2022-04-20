/// BufferWriter.hh - This file is a part of oslib-9 (not project)
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

#include <string>

#include "Buffer.hh"
#include "src_1.hh"

class BufferWriter {
public:
    BufferWriter(
        std::string&& name, Buffer& buffer,
        char a, char b)
        : name(name)
        , dist(a, b)
        , buffer(buffer)
    {
    }

    auto operator()() -> void
    {
        while (RUNNING) {
            auto c { this->gen() };

#pragma mark - sync
            {
                auto buf { this->buffer.lock() };
                buf->write(c);

                mvprintw(0, 0, "%s write '%c'!",
                    this->name.c_str(), c);
                refresh();
            }
#pragma mark - sync

            using namespace std;
            this_thread::sleep_for(100ms);
        }
    }

private:
    auto gen() -> char
    {
        return this->dist(RANDOM_DEVICE);
    }

    std::string name;
    std::uniform_int_distribution<char> dist;
    Buffer& buffer;
};
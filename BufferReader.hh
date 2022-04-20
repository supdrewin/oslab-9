/// BufferReader.hh - This file is a part of oslib-9 (not project)
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

#include <list>
#include <string>

#include "Buffer.hh"
#include "public.hh"

class BufferReader {
public:
    BufferReader(std::string&& name, Buffer& buffer,
        std::list<Range<char>>&& range)
        : name(name)
        , ranges(range)
        , buffer(buffer)
    {
    }

    auto operator()() -> void
    {
        while (RUNNING) {
#pragma mark - sync
            {
                auto buf { this->buffer.lock() };
                auto c { buf->take_if(cmp) };

                if ('\0' != c) {
                    mvprintw(1, 0, "%s take '%c'!",
                        this->name.c_str(), c);
                    refresh();
                }
            }
#pragma mark - sync

            using namespace std;
            this_thread::sleep_for(100ms);
        }
    }

private:
    std::function<bool(char)> cmp = [&](char c) {
        for (auto const& range : this->ranges) {
            if (c >= range.first && c <= range.second) {
                return true;
            }
        }
        return false;
    };

    std::string name;
    std::list<Range<char>> ranges;
    Buffer& buffer;
};

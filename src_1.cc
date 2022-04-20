/// src_1.cc - This file is a part of oslib-9 (not project)
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

#include <utility>

#include "Buffer.hh"
#include "BufferReader.hh"
#include "BufferWriter.hh"
#include "src_1.hh"

auto main() -> int
{
    Buffer buffer;

    BufferWriter bw1("Buffer Writer 1", buffer, 'A', 'Z');
    BufferWriter bw2("Buffer Writer 2", buffer, 'a', 'z');

    auto rng1 { std::make_pair('A', 'Z') };
    auto rng2 { std::make_pair('a', 'z') };

    BufferReader br1("Buffer Reader 1", buffer, { ref(rng1) });
    BufferReader br2("Buffer Reader 2", buffer, { ref(rng2) });

    BufferReader br3("Buffer Reader 3", buffer,
        {
            move(rng1),
            move(rng2),
        });

    auto win = initscr();
    noecho();

    use_window(win, call_back, nullptr);

    std::thread threads[] {
        std::thread(bw1),
        std::thread(bw2),
        std::thread(br1),
        std::thread(br2),
        std::thread(br3),
    };

    while (RUNNING) {
        if ('q' == getch()) {
            RUNNING = false;
        }
    }

    for (auto& thread : threads) {
        thread.join();
    }

    return endwin();
}

/// src_2.cc - This file is a part of oslab-9 (not project)
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

#include "Chair.hh"
#include "Chairs.hh"
#include "Tony.hh"
#include "public.hh"

auto main() -> int
{
    auto rng { std::make_pair(200, 5000) };
    Chairs<4> chairs(rng, rng);

    auto win = initscr();
    noecho();

    use_window(win, call_back, nullptr);

    std::thread threads[] {
        std::thread(chairs.push),
        std::thread(chairs.pop),
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

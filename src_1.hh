/// src_1.hh - This file is a part of oslib-9 (not project)
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

#pragma comment(lib, "curses")
#pragma comment(lib, "pthread")

#include <curses.h>
#include <random>
#include <thread>

static std::random_device RANDOM_DEVICE;
static auto RUNNING { true };

static auto call_back(WINDOW* win, void*)
    -> int
{
    mvprintw(win->_regbottom, 0,
        "Press 'q' to exit...");

    return refresh() == OK;
}

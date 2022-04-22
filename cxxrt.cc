/// cpprt.cc - This file is a part of oslab-9 (not project)
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

#include "cxxrt.hh"
#include "public.hh"

auto resize_event(StopToken stop_token,
    WINDOW* win) -> void
{
    decltype(win->_regbottom) bottom { 0 };

    while (!stop_token.stop_requested()) {
        if (win->_regbottom != bottom) {
            PRINTER.process(clear);
            PRINTER.process(mvprintw,
                bottom = win->_regbottom, 0,
                "Press 'q' to exit...");

            using namespace std;
            this_thread::sleep_for(
                PRINTER.refresh_rate());
        }
    }
}

auto refresh_event(StopToken stop_token,
    Printer* self) -> void
{
    while (!stop_token.stop_requested()) {
        using namespace std;
        this_thread::sleep_for(
            self->refresh_rate());

        self->process(refresh);
    }
}

auto main() -> int
{
#pragma mark - cfg
    auto window = initscr();

    PRINTER.process(cbreak);
    PRINTER.process(noecho);

    PRINTER.set_resize_event(resize_event, window);
    PRINTER.set_refresh_event(refresh_event, &PRINTER);
#pragma mark - cfg

#pragma mark - run
    {
        CppRtData threads;

        while ('q' != getch()) {
        }
#pragma mark - run

#pragma mark - end
    }

    PRINTER.wait_to_stop();
    return endwin();
#pragma mark - end
}

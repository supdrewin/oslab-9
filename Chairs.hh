/// Chairs.hh - This file is a part of oslab-9 (not project)
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
#include <deque>
#include <functional>
#include <mutex>

#include "Chair.hh"
#include "Tony.hh"
#include "public.hh"

template <size_t N>
class Chairs {
public:
    std::function<void()> push;
    std::function<void()> pop;

    Chairs(Range<size_t>&& add_msec,
        Range<size_t>&& del_msec)
        : push([&]() {
            size_t i { 0 }, n { 0 };

            while (RUNNING) {
                if (this->add_person(i, n)) {
                    i += 1;
                    i %= N;
                }

                auto msec { this->add_dist(RANDOM_DEVICE) };

                using namespace std;
                this_thread::sleep_for(
                    chrono::milliseconds(msec));
            }
        })
        , pop([&]() {
            size_t i { 0 };

            while (RUNNING) {
                if (this->del_person(i)) {
                    i += 1;
                    i %= N;
                }

                using namespace std;
                this_thread::sleep_for(100ms);
            }
        })
        , print_mutex()
        , add_dist(add_msec.first, add_msec.second)
        , del_dist(del_msec.first, del_msec.second)
        , chairs()
        , tony()
    {
    }

    auto add_person(size_t i, size_t& n) -> bool
    {
        if (N == (**this).size()) {
            this->locked_mvprintw([&]() {
                mvprintw(N, 0,
                    "%zu people left angrily!", ++n);
            });

            return false;
        }

        auto needed_time { this->del_dist(RANDOM_DEVICE) };

        this->locked_mvprintw([=]() {
            mvprintw(i, 0,
                "Add person, needed time: %zums.",
                needed_time);
        });

        (**this).push_back({
            .needed_time = needed_time,
        });

        return true;
    }

    auto del_person(size_t i) -> bool
    {
        if ((**this).empty()) {
            this->locked_mvprintw([]() {
                mvprintw(N + 1, 0, "Tony is sleeping...");
            });

            return false;
        }

        this->locked_mvprintw([]() {
            mvprintw(N + 1, 0, "Tony is working...");
        });

        auto tony_working { this->tony.wake_up() };
        tony_working(chairs.front().needed_time);

        chairs.pop_front();

        this->locked_mvprintw([=]() {
            mvprintw(i, 0, "Del");
        });

        return true;
    }

    auto operator*() -> std::deque<Chair>&
    {
        return this->chairs;
    }

private:
    std::mutex print_mutex;

    auto locked_mvprintw(
        std::function<void()> mvprintw)
        -> void
    {
        this->print_mutex.lock();

        mvprintw();
        refresh();

        this->print_mutex.unlock();
    }

    std::uniform_int_distribution<size_t> add_dist;
    std::uniform_int_distribution<size_t> del_dist;

    std::deque<Chair> chairs;
    Tony tony;
};

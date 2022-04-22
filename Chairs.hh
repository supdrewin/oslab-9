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

#include "Chair.hh"
#include "Tony.hh"
#include "public.hh"

template <size_t N = 4>
class Chairs {
public:
    using DistType = std::uniform_int_distribution<size_t>;

    Chairs(Range<size_t>&& add_msec,
        Range<size_t>&& del_msec)
        : add_dist(add_msec.first, add_msec.second)
        , del_dist(del_msec.first, del_msec.second)
        , chairs()
        , tony()
    {
    }

    auto size() const noexcept
        -> size_t
    {
        return N;
    }

    auto get_add_dist() -> DistType&
    {
        return this->add_dist;
    }

    auto get_del_dist() -> DistType&
    {
        return this->del_dist;
    }

    auto add_person(size_t i, size_t& n) -> bool
    {
        if (N == (**this).size()) {
            PRINTER.process(
                mvprintw, N, 0,
                "%zu people left angrily!",
                ++n);

            return false;
        }

        auto needed_time {
            this->del_dist(RANDOM_DEVICE)
        };

        PRINTER.process(
            mvprintw, i, 0,
            "Add person, needed time: %zums.",
            needed_time);

        (**this).push_back({
            .needed_time = needed_time,
        });

        return true;
    }

    auto del_person(size_t i) -> bool
    {
        if ((**this).empty()) {
            PRINTER.process(mvprintw, N + 1, 0,
                "Tony is sleeping...");

            return false;
        }

        PRINTER.process(mvprintw, N + 1, 0,
            "Tony is working...");

        auto tony_working { this->tony.wake_up() };
        auto needed_time {
            chairs.front().needed_time
        };

        tony_working(needed_time);

        chairs.pop_front();

        PRINTER.process(
            mvprintw, i, 0,
            "Del person, needed time: %zums.",
            needed_time);

        return true;
    }

    auto operator*() -> std::deque<Chair>&
    {
        return this->chairs;
    }

private:
    DistType add_dist;
    DistType del_dist;

    std::deque<Chair> chairs;
    Tony tony;
};

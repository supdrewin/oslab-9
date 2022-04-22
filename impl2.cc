/// impl2.cc - This file is a part of oslab-9 (not project)
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

#include <cstddef>

#include "Chairs.hh"
#include "Thread.hh"
#include "cxxrt.hh"

auto push_event(StopToken stop_token,
    Chairs<>* self) -> void
{
    size_t i { 0 }, n { 0 };

    while (!stop_token.stop_requested()) {
        if (self->add_person(i, n)) {
            i = (i + 1) % self->size();
        }

        auto msec {
            self->get_add_dist()(RANDOM_DEVICE)
        };

        using namespace std;
        this_thread::sleep_for(
            chrono::milliseconds(msec));
    }
}

auto pop_event(StopToken stop_token,
    Chairs<>* self) -> void
{
    size_t i { 0 };

    while (!stop_token.stop_requested()) {
        if (self->del_person(i)) {
            i = (i + 1) % self->size();
        }

        using namespace std;
        this_thread::sleep_for(100ms);
    }
}

class ImplRtData {
public:
    ImplRtData(Range<size_t>&& a,
        Range<size_t>&& b)
        : chairs(move(a), move(b))
        , push()
        , pop()
    {
    }

    auto run() -> void
    {
        JThread push(push_event, &this->chairs);
        JThread pop(pop_event, &this->chairs);

        this->push.swap(push);
        this->pop.swap(pop);
    }

    auto stop() -> void
    {
        this->push.request_stop();
        this->pop.request_stop();
    }

private:
    Chairs<> chairs;
    JThread push;
    JThread pop;
} impl_rt_data(
    std::make_pair(200, 5000),
    std::make_pair(200, 5000));

CppRtData::CppRtData()
{
    impl_rt_data.run();
}

CppRtData::~CppRtData()
{
    impl_rt_data.stop();
}

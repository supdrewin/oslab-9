/// impl1.cc - This file is a part of oslab-9 (not project)
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
#include "cpprt.hh"

class ImplRtData {
public:
    ImplRtData()
        : buffer()
        , buffer_writer_1("Buffer Writer 1", buffer, 'A', 'Z')
        , buffer_writer_2("Buffer Writer 2", buffer, 'a', 'z')
        , buffer_reader_1("Buffer Reader 1", buffer,
              {
                  std::make_pair('A', 'Z'),
              })
        , buffer_reader_2("Buffer Reader 2", buffer,
              {
                  std::make_pair('a', 'z'),
              })
        , buffer_reader_3("Buffer Reader 3", buffer,
              {
                  std::make_pair('A', 'Z'),
                  std::make_pair('a', 'z'),
              })
        , threads()

    {
    }

    auto run() -> void
    {
        std::jthread threads[] {
            std::jthread(buffer_writer_1),
            std::jthread(buffer_writer_2),
            std::jthread(buffer_reader_1),
            std::jthread(buffer_reader_2),
            std::jthread(buffer_reader_3),

        };
        std::swap(this->threads, threads);
    }

    auto stop() -> void
    {
        for (auto& thread : this->threads) {
            thread.request_stop();
        }
    }

private:
    Buffer buffer;

    BufferWriter buffer_writer_1;
    BufferWriter buffer_writer_2;

    BufferReader buffer_reader_1;
    BufferReader buffer_reader_2;
    BufferReader buffer_reader_3;

    std::jthread threads[5];
} impl_rt_data;

CppRtData::CppRtData()
{
    impl_rt_data.run();
}

CppRtData::~CppRtData()
{
    impl_rt_data.stop();
}

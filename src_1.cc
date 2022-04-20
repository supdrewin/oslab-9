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

#include <curses.h>
#include <deque>
#include <functional>
#include <list>
#include <memory>
#include <mutex>
#include <random>
#include <string>
#include <thread>
#include <utility>

#pragma comment(lib, "curses")
#pragma comment(lib, "pthread")

static std::random_device RANDOM_DEVICE;
static auto RUNNING { true };

class BufferHelper {
public:
    BufferHelper(std::deque<char>* deque,
        std::mutex* mutex)
        : deque(deque)
        , mutex(mutex)
    {
    }

    ~BufferHelper()
    {
        this->mutex->unlock();
    }

    auto read() -> char
    {
        return (**this).empty()
            ? '\0'
            : (**this).front();
    }

    auto take_if(std::function<bool(char)> cmp)
        -> char
    {
        auto c = this->read();

        if (cmp(c)) {
            (**this).pop_front();
            return c;
        }

        return '\0';
    }

    auto write(char c) -> void
    {
        (**this).push_back(c);
    }

    auto operator*() -> std::deque<char>&
    {
        return *this->deque;
    }

private:
    std::deque<char>* deque;
    std::mutex* mutex;
};

class Buffer {
public:
    auto lock() -> std::unique_ptr<BufferHelper>
    {
        this->mutex.lock();
        return std::unique_ptr<BufferHelper> {
            new BufferHelper(&deque, &mutex)
        };
    }

    auto try_lock() -> std::unique_ptr<BufferHelper>
    {
        return std::unique_ptr<BufferHelper> {
            this->mutex.try_lock()
                ? new BufferHelper(&deque, &mutex)
                : nullptr
        };
    }

private:
    std::deque<char> deque;
    std::mutex mutex;
};

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

class BufferReader {
public:
    BufferReader(std::string&& name, Buffer& buffer,
        std::list<std::pair<char, char>>&& range)
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
    std::list<std::pair<char, char>> ranges;
    Buffer& buffer;
};

auto call_back(WINDOW* win, void*)
    -> int
{
    mvprintw(win->_regbottom, 0,
        "Press 'q' to exit...");

    return refresh() == OK;
}

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

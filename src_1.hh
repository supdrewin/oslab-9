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

#pragma once

#include <ncurses.h>
#include "Dimension.h"

struct WindowPainter
{
    WINDOW* win;

    WindowPainter(Dimension bounds)
    {
        win = newwin(bounds.y, bounds.x, bounds.height, bounds.width);
    }

    ~WindowPainter()
    {
        delwin(win);
    }

    virtual void draw()
    {
        wclear(win);
        wprintw(win, "Hello");
    }

    virtual void show()
    {
        wrefresh(win);
    }

    bool resize(Dimension bounds) {
        delwin(win);
        win = newwin(bounds.y, bounds.x, bounds.height, bounds.width);
        return true;
    }
};

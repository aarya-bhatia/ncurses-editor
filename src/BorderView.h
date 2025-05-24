#pragma once

#include "Window.h"
#include <ncurses.h>

struct BorderView : public Window {
    WINDOW* win = NULL;
    bool dirty = true;

    BorderView(Dimension d) {
        _init(d);
    }

    ~BorderView() {
        _destroy();
    }

    void resize(Dimension d)
    {
        werase(win);
        wnoutrefresh(win);

        _destroy();
        _init(d);
    }

    void draw()
    {
        if (dirty) {
            // dirty = false;
            box(win, '|', '-');
        }

        wnoutrefresh(win);
    }

private:
    void _init(Dimension d) {
        if (d.width > 0 && d.height > 0) {
            win = newwin(d.height, d.width, d.y, d.x);
            dirty = true;
            log_debug("border view dirty %s", d.debug_string().c_str());
        }
    }

    void _destroy() {
        delwin(win);
        win = NULL;
    }
};
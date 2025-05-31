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

    void resize(Dimension d) override
    {
        hide();
        _destroy();
        _init(d);
    }

    void draw() override
    {
        if (dirty) {
            dirty = false;
            box(win, '|', '-');
        }

        wnoutrefresh(win);
    }

    void hide() override {
        if (!win) { return; }
        werase(win);
        wnoutrefresh(win);
    }

    void show() override {
        dirty = true;
    }

private:
    void _init(Dimension d) {
        if (d.width > 0 && d.height > 0) {
            win = newwin(d.height, d.width, d.y, d.x);
            dirty = true;
        }
    }

    void _destroy() {
        if (!win) { return; }
        delwin(win);
        win = NULL;
    }
};
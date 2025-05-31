#pragma once

#include <ncurses.h>
#include <string>
#include <vector>
#include "log.h"

class Editor;

struct StatusWindow
{
    Editor& editor;
    Dimension bounds;
    WINDOW *win = NULL;

    StatusWindow(Editor& editor, Dimension d) : editor(editor), bounds(d) {
        win = newwin(d.height, d.width, d.y, d.x);
    }

    std::string get_status();

    void draw();
    void resize(Dimension bounds);
};
#pragma once

#include <ncurses.h>
#include <string>
#include "log.h"
#include "Editor.h"

struct ConsoleWindow
{
    Editor& editor;
    WINDOW* win = NULL;

    ConsoleWindow(Editor& editor, Dimension d) : editor(editor) {
        this->win = newwin(d.height, d.width, d.y, d.x);
    }

    void draw() {
        werase(win);

        int ncols = getmaxx(win);

        std::string tmp = editor.get_console_string();
        mvwprintw(win, 0, 0, "%s", tmp.substr(0, ncols).c_str());
        wnoutrefresh(win);
    }

    void resize(Dimension d)
    {
        werase(win);
        wnoutrefresh(win);
        delwin(win);
        this->win = newwin(d.height, d.width, d.y, d.x);
    }
};
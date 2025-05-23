#pragma once
#include <ncurses.h>
#include "Dimension.h"

struct NcursesWindow {
    WINDOW* window = nullptr;

    NcursesWindow() {}

    NcursesWindow(Dimension d) {
        window = newwin(d.height, d.width, d.y, d.x);
    }

    int height() const { return getmaxy(window); }
    int width() const { return getmaxx(window); }

    WINDOW* get() const noexcept { return window; }

    bool is_valid_position(int y, int x) {
        return x >= 0 && y >= 0 && x < width() && y < height();
    }

    void show()
    {
        if (window) {
            wrefresh(window);
        }
    }

    void draw_line(int y, const std::string& content) {
        if (is_valid_position(y, 0)) {
            mvwprintw(window, y, 0, content.substr(0, width()).c_str());
        }
    }

    void draw_till_eol(int y, int x, const std::string& content) {
        if (is_valid_position(y, x)) {
            mvwprintw(window, y, x, content.substr(0, width() - x).c_str());
        }

    }

    void draw_character(int y, int x, char c) {
        if (is_valid_position(y, x)) {
            mvwaddch(window, y, x, c);
        }

    }

    void clear() {
        wclear(window);

    }

    void clear_line(int y) {
        wmove(window, y, 0);
        wclrtoeol(window);

    }

    void clear_till_eol(int y, int x)
    {
        wmove(window, y, x);
        wclrtoeol(window);
    }

    void move(int y, int x) {
        wmove(window, y, x);
    }

    void move_cursor(int y, int x) {
        move(y, x);
    }

    void draw_border() {
        if (window) {
            box(window, '|', '-');
        }
    }
};


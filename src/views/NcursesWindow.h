#pragma once
#include <ncurses.h>
#include <memory>
#include "Dimension.h"
#include "Painter.h"

struct WindowDeleter {
    void operator()(WINDOW* window) const {
        if (window) {
            delwin(window);
        }
    }
};

struct NcursesWindow : public Painter {
    std::unique_ptr<WINDOW, WindowDeleter> window;

    NcursesWindow() {
    }

    NcursesWindow(Dimension dim) {
        window = std::unique_ptr<WINDOW, WindowDeleter>(
            newwin(dim.height, dim.width, dim.y, dim.x)
        );

        wrefresh(window.get());
    }

    int height() const { return getmaxy(window.get()); }
    int width() const { return getmaxx(window.get()); }

    WINDOW* get() const noexcept { return window.get(); }

    bool is_valid_position(int y, int x) {
        return x >= 0 && y >= 0 && x < width() && y < height();
    }

    void show()
    {
        if(window) {
            wrefresh(window.get());
        }
    }

    void draw_line(int y, const std::string& content) override;
    void draw_till_eol(int y, int x, const std::string& content) override;
    void draw_character(int y, int x, char c) override;
    void clear() override;
    void clear_line(int y) override;
    void clear_till_eol(int y, int x) override;
    void move(int y, int x) override;

    void move_cursor(int y, int x) override {
        move(y, x);
    }

    void draw_border() {
        if(window) {
            box(window.get(), '|', '-');
        }
    }
};


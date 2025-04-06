#pragma once

#include <ncurses.h>
#include "File.h"
#include "window/ContentWindow.h"

struct WindowDeleter {
    void operator()(WINDOW* window) const {
        if (window) {
            delwin(window);
        }
    }
};

struct NcursesWindow {
    std::unique_ptr<WINDOW, WindowDeleter> window;

    NcursesWindow() {
    }

    NcursesWindow(Dimension dim) {
        window = std::unique_ptr<WINDOW, WindowDeleter>(
            newwin(dim.height, dim.width, dim.y, dim.x)
        );
    }

    WINDOW* get() const noexcept { return window.get(); }
};

struct FileView : public ContentWindow
{
    std::shared_ptr<File> file;
    NcursesWindow window;
    Scroll page_scroll;

    FileView(const std::shared_ptr<File>& file, Dimension bounds = Dimension()) : ContentWindow(bounds), file(file)
    {
    }

    void draw() override {
        if (window.get()) {
            wclear(window.get());
            wprintw(window.get(), "Test");
        }
    }

    void show() override {
        if (window.get()) {
            wrefresh(window.get());
        }
    }

    void resize(Dimension bounds) override {
        if (!ContentWindow::resizable(bounds)) {
            return;
        }
        ContentWindow::resize(bounds);
        window = NcursesWindow(bounds);
    }

    int get_relative_y(int absy) const
    {
        return absy - page_scroll.dy;
    }

    int get_relative_x(int absx) const
    {
        return absx - page_scroll.dx;
    }

    int get_display_y(int absy) const
    {
        return get_relative_y(absy) - page_scroll.dy;
    }

    int get_display_x(int absx) const
    {
        return get_relative_x(absx) - page_scroll.dx;
    }

    int height() const
    {
        return getmaxy(window.get());
    }

    int width() const
    {
        return getmaxx(window.get());
    }

    bool adjust_page_scroll()
    {
        Cursor& cursor = file->cursor;

        // adjust horizontal page_scroll
        if (cursor.x - page_scroll.dx < 0)
        {
            log_debug("page_scrolling left");
            page_scroll.dx = cursor.x;
            return true;
        }
        else if (cursor.x - page_scroll.dx >= width())
        {
            log_debug("page_scrolling right");
            page_scroll.dx = cursor.x - width() + 1;
            return true;
        }

        // adjust vertical page_scroll
        if (cursor.y - page_scroll.dy < 0)
        {
            log_debug("page_scrolling up");
            page_scroll.dy = cursor.y;
            return true;
        }
        else if (cursor.y - page_scroll.dy >= height())
        {
            log_debug("page_scrolling down");
            page_scroll.dy = cursor.y - height() + 1;
            return true;
        }

        return false;
    }
};
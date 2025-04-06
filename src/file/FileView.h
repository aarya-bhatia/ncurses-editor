#pragma once

#include <ncurses.h>
#include "File.h"
#include "window/ContentWindow.h"
#include "NcursesWindow.h"

struct FileView : public ContentWindow
{
    std::shared_ptr<File> file;
    NcursesWindow window;
    Scroll page_scroll;

    FileView(const std::shared_ptr<File>& file, Dimension bounds = Dimension()) : ContentWindow(bounds), file(file)
    {
    }

    ContentType get_content_type() override { return ContentType::FileContent; }

    std::shared_ptr<void> get_model() override {
        return file;
    }

    void draw() override {
        if (window.get()) {
            wclear(window.get());
            wprintw(window.get(), "Test");
        }

        // werase(edit_window);

        // if (!file)
        // {
        //     return;
        // }
        // Scroll& scroll = file->scroll;
        // auto& lines = file->lines;

        // auto line_itr = lines.begin();
        // std::advance(line_itr, scroll.dy);
        // int count_lines = 0;
        // int max_lines, max_cols;
        // getmaxyx(edit_window, max_lines, max_cols);
        // for (; line_itr != lines.end() && count_lines < max_lines; line_itr++, count_lines++)
        // {
        //     wmove(edit_window, count_lines, 0);
        //     auto& line = *line_itr;
        //     auto col_itr = line.begin();
        //     std::advance(col_itr, scroll.dx);
        //     int count_cols = 0;
        //     for (; col_itr != line.end() && count_cols < max_cols; col_itr++, count_cols++)
        //     {
        //         waddch(edit_window, *col_itr);
        //     }
        // }
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
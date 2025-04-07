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

    int get_absolute_y(int rely) const
    {
        return rely + bounds.y;
    }

    int get_absolute_x(int relx) const
    {
        return relx + bounds.x;
    }

    int get_display_y(int rely) const
    {
        return rely - page_scroll.dy;
    }

    int get_display_x(int relx) const
    {
        return relx - page_scroll.dx;
    }

    int height() const
    {
        return bounds.height;
    }

    int width() const
    {
        return bounds.width;
    }

    bool scroll_to_ensure_cursor_visible();
};

// auto file = get_current_file();
// if (!file)
// {
//     return;
// }
// Scroll& scroll = file->scroll;

// int display_line = info.y - scroll.dy;
// if (display_line < 0 || display_line >= getmaxy(edit_window))
// {
//     return;
// }

// wmove(edit_window, display_line, 0);
// wclrtoeol(edit_window);
// int max_cols = getmaxx(edit_window);

// std::list<char>& line = *info.line;
// auto col_itr = line.begin();
// std::advance(col_itr, scroll.dx);
// int count_cols = 0;
// for (; col_itr != line.end() && count_cols < max_cols; col_itr++, count_cols++)
// {
//     waddch(edit_window, *col_itr);
// }
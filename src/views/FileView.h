#pragma once

#include <ncurses.h>
#include "File.h"
#include "FileSubscriber.h"
#include "Window.h"
#include "NcursesWindow.h"

struct FileView : public Window, FileSubscriber
{
    Dimension bounds;
    File* file = nullptr;
    NcursesWindow window;
    Scroll scroll;
    bool redraw = true;

    int save_cursor_y = 0;
    int save_cursor_x = 0;

    FileView(File* file, Dimension bounds);

    void on_file_reload(File& file) override {
        redraw = true;
    }

    bool is_visible(int y, int x) const {
        return y >= 0 && x >= 0 && y < height() && x < width();
    }

    void partial_draw_character(Cursor position);
    void partial_draw_line(Cursor position);
    void on_insert_character(File& file, Cursor position, char c) override;
    void on_erase_character(File& file, Cursor position) override;
    void on_replace_character(File& file, Cursor position) override;

    void draw() override;

    void show() override {
        if (window.get()) {
            wrefresh(window.get());
        }
    }

    void resize(Dimension bounds) override;

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
        return rely - scroll.dy;
    }

    int get_display_x(int relx) const
    {
        return relx - scroll.dx;
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

    void draw_cursor();
    void draw_content();

    void focus() override {}
    void unfocus() override {}

    Window* copy(Dimension d) override {
        return new FileView(file, d);
    }
};
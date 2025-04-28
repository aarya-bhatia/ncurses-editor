#pragma once

#include <ncurses.h>
#include "File.h"
#include "FileSubscriber.h"
#include "Window.h"
#include "NcursesWindow.h"

struct FileView : public Window
{
    Dimension bounds;
    File* file = nullptr;
    NcursesWindow window;
    Scroll scroll;
    bool should_redraw = true;
    bool focused = false;

    int save_cursor_y = 0;
    int save_cursor_x = 0;

    int redraw_count = 0;

    FileView(File* file, Dimension bounds);

    Dimension get_bounds() { return bounds; }

    bool is_visible(int y, int x) const {
        return y >= 0 && x >= 0 && y < height() && x < width();
    }

    void redraw() { should_redraw = true; }

    void partial_draw_character(Cursor position);
    void partial_draw_line(Cursor position);

    void draw();

    void show() {
        window.show();
    }

    void resize(Dimension bounds);

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
    void focus();
    void unfocus();

    File* get_file() { return file; }
    void force_redraw() { should_redraw = true; }

    void clear() {
        should_redraw = true;
        window.clear();
    }
};
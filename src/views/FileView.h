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
    bool redraw = true;
    bool focused = false;

    int save_cursor_y = 0;
    int save_cursor_x = 0;

    FileView(File* file, Dimension bounds);

    bool is_visible(int y, int x) const {
        return y >= 0 && x >= 0 && y < height() && x < width();
    }

    void partial_draw_character(Cursor position) override;
    void partial_draw_line(Cursor position) override;

    void draw() override;

    void show() override {
        window.show();
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

    void focus() override {
        focused = true;
    }

    void unfocus() override {
        focused = false;
    }

    Window* copy(Dimension d) override {
        return new FileView(file, d);
    }

    File* get_file() override { return file; }

    void force_redraw() override { redraw = true; }

    void clear() override {
        redraw = true;
        window.clear();
    }
};
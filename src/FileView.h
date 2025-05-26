#pragma once

#include "File.h"
#include "Window.h"
#include <ncurses.h>

struct FileView : public Window
{
    FileView(File* file, Dimension d);
    ~FileView() { delwin(win); }

    File* get_file() { return file; }

    void draw() override;
    void resize(Dimension bounds) override;
    int height() const { return getmaxy(win); }
    int width() const { return getmaxx(win); }
    void partial_draw_character(Cursor position);
    void partial_draw_line(Cursor position);
    void focus() override;
    void unfocus() override;
    void hide() override;
    void show() override;
    void set_dirty() { dirty = true; }

private:

    // check if given coordinate is visible on screen
    bool is_visible(int y, int x) const { return y >= 0 && x >= 0 && y < height() && x < width(); }

    bool scroll_to_ensure_cursor_visible();

    Dimension bounds;

    WINDOW* win = NULL;
    File* file = nullptr; // shared ptr
    Scroll scroll;
    bool dirty = true;
    bool focused = false;

    // save position of file cursor before this view loses focus
    int save_cursor_y = 0;
    int save_cursor_x = 0;
};
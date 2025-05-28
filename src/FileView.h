#pragma once

#include "File.h"
#include "FileSubscriber.h"
#include "Window.h"
#include <ncurses.h>

struct FileView : public Window, public FileSubscriber
{
    FileView(File* file, Dimension d);
    ~FileView();

    File* get_file() { return file; }

    void draw() override;
    void resize(Dimension bounds) override;
    int height() const { return getmaxy(win); }
    int width() const { return getmaxx(win); }
    void focus() override;
    void unfocus() override;
    void hide() override;
    void show() override;
    void set_dirty() { dirty = true; }
    void get_absolute_cursor(int& y, int& x);

    void file_changed() override {
        dirty = true;
    }

    void line_added(Cursor at)  override {
        _partial_draw_file(at);
    }

    void line_removed(Cursor at)  override {
        _partial_draw_file(at);
    }

    void character_added(Cursor at)  override {
        _partial_draw_line(at);
    }

    void character_removed(Cursor at)  override {
        _partial_draw_line(at);
    }

    Scroll get_scroll() const { return scroll; }

    bool scroll_to_ensure_cursor_visible();

private:

    void _draw_file_content(WINDOW* win);

    void _partial_draw_character(Cursor position);
    void _partial_draw_line(Cursor position);
    void _partial_draw_file(Cursor start);

    void _init(Dimension d);
    void _destroy();

    // check if given coordinate is visible on screen
    bool is_visible(int y, int x) const { return y >= 0 && x >= 0 && y < height() && x < width(); }

private:
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
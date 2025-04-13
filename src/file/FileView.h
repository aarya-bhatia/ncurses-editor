#pragma once

#include <ncurses.h>
#include "window/ContentWindow.h"
#include "file/File.h"
#include "file/FileSubscriber.h"
#include "NcursesWindow.h"

struct FileView : public ContentWindow, FileSubscriber
{
    std::shared_ptr<File> file;
    NcursesWindow window;
    Scroll page_scroll;
    bool redraw = true;

    int save_cursor_y = 0;
    int save_cursor_x = 0;

    FileView(const std::shared_ptr<File>& file, Dimension bounds = Dimension());

    FileView(const FileView& other) :
        FileView(other.file, other.bounds) {
    }

    void on_focus() override;
    void on_unfocus() override;

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

    ContentType get_content_type() override { return ContentType::FileContent; }

    std::shared_ptr<void> get_model() override {
        return file;
    }

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

    void draw_cursor();
    void draw_content();
};
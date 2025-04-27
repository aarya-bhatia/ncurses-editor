#pragma once

#include "FileView.h"

struct BorderedFileView : public Window {
    Dimension bounds;
    FileView file_view;
    NcursesWindow frame;
    bool redraw = true;

    BorderedFileView(File* file, Dimension d) : bounds(d), file_view(file, get_child_bounds()), frame(bounds) {
    }

    Dimension get_child_bounds() const {
        return Dimension(bounds.x + 1, bounds.y + 1, bounds.width - 2, bounds.height - 2);
    }

    Dimension get_bounds() { return bounds; }

    void resize(Dimension d)
    {
        this->bounds = d;
        file_view.resize(get_child_bounds());
        frame = NcursesWindow(bounds);
        redraw = true;
    }

    void draw()
    {
        if (redraw) {
            frame.draw_border();
            file_view.draw();
            // TODO: redraw = false;
        }
    }

    void show()
    {
        frame.show();
        file_view.show();
    }

    void focus() {
        log_info("focus window %s", bounds.debug_string().c_str());
        file_view.focus();
    }

    void unfocus() {
        file_view.unfocus();
    }

    Window* copy(Dimension d) {
        return new BorderedFileView(file_view.file, d);
    }

    File* get_file() { return file_view.file; }

    void partial_draw_character(Cursor position) {
        file_view.partial_draw_character(position);
    }

    void partial_draw_line(Cursor position) {
        file_view.partial_draw_line(position);
    }

    void force_redraw() {
        redraw = true;
        file_view.force_redraw();
    }

    void clear() {
        log_info("Clearing window %s", bounds.debug_string().c_str());
        redraw = true;
        frame.clear();
        file_view.clear();
    }
};
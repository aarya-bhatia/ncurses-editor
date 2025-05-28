#pragma once

#include "Window.h"
#include <ncurses.h>
#include "Scroll.h"
#include "FileSubscriber.h"

struct LineNumberView : public Window, public FileSubscriber
{
    WINDOW* win = NULL;
    bool dirty = true;
    Dimension bounds;
    File* file = NULL;
    FileView* file_view = NULL;
    int top_line = 0;

    LineNumberView(Dimension d, File* f, FileView* fv) : bounds(d), file(f), file_view(fv) {
        win = newwin(d.height, d.width, d.y, d.x);
        file->add_subscriber((FileSubscriber*)this);
    }

    ~LineNumberView() {
        file->remove_subscriber((FileSubscriber*)this);
    }

    void file_changed() {
        dirty = true;
    }

    void line_added(Cursor) {
        dirty = true;
    }

    void line_removed(Cursor) {
        dirty = true;
    }

    void draw() {
        if (top_line != file_view->get_scroll().dy) { dirty = true; }
        top_line = file_view->get_scroll().dy;

        if (!dirty) { return; }
        dirty = false;
        werase(win);

        int i;
        for (i = 0; i < bounds.height && i < file->count_lines(); i++) {
            wmove(win, i, 0);
            wprintw(win, "%d", top_line + i + 1);
        }

        for (; i < bounds.height; i++) {
            wmove(win, i, 0);
            waddch(win, '~');
        }

        wnoutrefresh(win);
    }

    void resize(Dimension d) {
        hide();
        delwin(win);
        win = newwin(d.height, d.width, d.y, d.x);
        dirty = true;
    }

    void hide() {
        werase(win);
        wnoutrefresh(win);
    }

    void show() {
        dirty = true;
    }

    static int get_max_width(int nlines) {
        if (nlines <= 0) return 2;  // Handles 0 and negative numbers as single digit

        int width = 2;
        while (nlines > 0) {
            nlines /= 10;
            ++width;
        }
        return width;
    }

};
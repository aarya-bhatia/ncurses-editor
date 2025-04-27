#pragma once

#include "BorderView.h"
#include "FileView.h"

struct ViewContainer : public Window
{
    BorderView* boundary = nullptr;
    FileView* file_view = nullptr;
    bool focused = false;

    ViewContainer(Dimension d) {
        boundary = new BorderView(d);
    }

    File* get_file() {
        return boundary->file;
    }

    void set_file(File* _file) {
        boundary->file = _file;
    }

    Dimension get_bounds() {
        return boundary->bounds;
    }

    ~ViewContainer() {
        delete boundary;
        delete file_view;
    }

    void draw() {
        boundary->draw();

        if (focused && !file_view) {
            move(boundary->bounds.y + 1, boundary->bounds.x + 1);
        }
    }

    void show() {
        boundary->show();
    }

    void resize(Dimension d) {
        boundary->resize(d);
    }

    void redraw() {
        boundary->redraw();
    }

    void focus() {
        focused = true;
    }

    void unfocus() {
        focused = false;
    }
};
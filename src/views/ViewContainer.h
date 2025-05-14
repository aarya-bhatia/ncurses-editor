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

    bool is_composite() override { return true; }

    void set_file_view(FileView* _file_view) override {
        if (file_view == _file_view) { return; }
        if (file_view) {
            file_view->unfocus();
        }
        file_view = _file_view;
        resize(boundary->bounds);
        if (file_view) {
            _file_view->focus();
        }
    }

    FileView* get_file_view() override { return file_view; }

    Dimension get_bounds()override {
        return boundary->bounds;
    }

    ~ViewContainer() {
        delete boundary;
        delete file_view;
    }

    void draw() override {
        boundary->draw();
        if (file_view) file_view->draw();

        if (focused) {
            if (file_view) {
                file_view->draw_cursor();
            }
            else {
                move(boundary->bounds.y + 1, boundary->bounds.x + 1);
            }
        }
    }

    void show()override {
        boundary->show();
        if (file_view) file_view->show();
    }

    void resize(Dimension d)override {
        boundary->resize(d);
        if (file_view) {
            if (d.width >= 2 && d.height >= 2) {
                file_view->resize(Dimension(d.x + 1, d.y + 1, d.width - 2, d.height - 2));
            }
            else {
                file_view->resize(Dimension(d.x + 1, d.y + 1, 0, 0));
            }
        }
    }

    void redraw() override {
        boundary->redraw();
        if (file_view) file_view->redraw();
    }

    void focus() override {
        focused = true;
        if (file_view) file_view->focus();
    }

    void unfocus() override {
        focused = false;
        if (file_view) file_view->unfocus();
    }

    void partial_draw_line(Cursor position) override
    {
        if (file_view) file_view->partial_draw_line(position);
    }

    void partial_draw_character(Cursor position) override
    {
        if (file_view) file_view->partial_draw_character(position);
    }
};
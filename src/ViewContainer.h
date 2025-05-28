#pragma once

#include "Window.h"
#include "BorderView.h"
#include "FileView.h"
#include "LineNumberView.h"

struct ViewContainer : public Window
{
private:
    Dimension bounds;
    BorderView* frame = nullptr;
    LineNumberView* line_view = nullptr;
    FileView* view = nullptr;
    File* file;
    bool focused = false;

    bool show_lines = false;
    Dimension file_view_bounds;
    Dimension line_view_bounds;

    Scroll prev_scroll;

public:
    ViewContainer(Dimension d, File* f) : file(f) {
        this->bounds = d;
        frame = new BorderView(d);

        _init(d, file);

        view = new FileView(file, file_view_bounds);

        if (show_lines) {
            line_view = new LineNumberView(line_view_bounds, file, view);
        }
    }

    void _init(Dimension d, File* file) {
        int max_no_width = LineNumberView::get_max_width(file->count_lines());

        if (d.width - max_no_width - 2 > 0) {
            show_lines = true;
            line_view_bounds = Dimension(d.x + 1, d.y + 1, max_no_width, d.height - 2);
            file_view_bounds = Dimension(d.x + 1 + max_no_width, d.y + 1, d.width - 2 - max_no_width, d.height - 2);
        }
        else {
            show_lines = false;
            file_view_bounds = Dimension(d.x + 1, d.y + 1, d.width - 2, d.height - 2);
        }
    }

    ~ViewContainer() {
        delete frame;
        delete view;
    }

    FileView* get_file_view() {
        return view;
    }

    void draw() override {
        frame->draw(); // frame must be drawn before content so that content does not get overriden
        if (focused) view->scroll_to_ensure_cursor_visible();
        if (line_view) line_view->draw();
        view->draw();
    }

    void resize(Dimension d)override {
        if (d == this->bounds) { return; }
        this->bounds = d;
        frame->resize(d);

        _init(d, file);

        if (show_lines) {
            if (line_view) {
                line_view->resize(line_view_bounds);
            }
            else {
                line_view = new LineNumberView(line_view_bounds, file, view);
            }
        }

        view->resize(file_view_bounds);
    }

    void focus() override {
        focused = true;
        frame->focus();
        view->focus();
    }

    void unfocus() override {
        focused = false;
        frame->unfocus();
        view->unfocus();
    }

    void hide() override {
        frame->hide();
        if (line_view) line_view->hide();
        view->hide();
    }

    void show() override {
        frame->show();
        if (line_view) line_view->show();
        view->show();
    }
};
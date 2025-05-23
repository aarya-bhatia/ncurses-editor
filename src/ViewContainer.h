#pragma once

#include "Window.h"
#include "BorderView.h"
#include "FileView.h"

struct ViewContainer : public Window
{
private:
    BorderView* frame = nullptr;
    Window* view = nullptr;
    bool focused = false;

public:
    ViewContainer(Dimension d) {
        frame = new BorderView(d);
    }

    ViewContainer(Dimension d, File* file) {
        frame = new BorderView(d);
        if (file) {
            view = new FileView(file, get_view_bounds());
        }
    }

    ~ViewContainer() {
        delete frame;
        delete view;
    }

    Dimension get_view_bounds() {
        Dimension d = frame->bounds;
        if (d.width >= 2 && d.height >= 2) {
            return Dimension(d.x + 1, d.y + 1, d.width - 2, d.height - 2);
        }
        else {
            return Dimension();
        }
    }

    void set_view(Window* _view) {
        delete view;
        view = _view;
        _view->resize(get_view_bounds());
    }

    Window* get_view() { return view; }

    Dimension get_bounds()override {
        return frame->bounds;
    }

    void draw() override {
        frame->draw();
        if (view) {
            view->draw();
        }
        else if (focused) {
            move(frame->bounds.y + 1, frame->bounds.x + 1);
        }
    }

    void show()override {
        frame->show();
        if (view) view->show();
    }

    void resize(Dimension d)override {
        frame->resize(d);
        if (view) {
            if (d.width >= 2 && d.height >= 2) {
                view->resize(Dimension(d.x + 1, d.y + 1, d.width - 2, d.height - 2));
            }
            else {
                view->resize(Dimension(d.x + 1, d.y + 1, 0, 0));
            }
        }
    }

    void redraw() override {
        frame->redraw();
        if (view) view->redraw();
    }

    void focus() override {
        focused = true;
        if (view) view->focus();
    }

    void unfocus() override {
        focused = false;
        if (view) view->unfocus();
    }

    void partial_draw_line(Cursor position) override
    {
        if (view) view->partial_draw_line(position);
    }

    void partial_draw_character(Cursor position) override
    {
        if (view) view->partial_draw_character(position);
    }

    void clear() override
    {
        frame->clear();
        if (view) view->clear();
    }
};
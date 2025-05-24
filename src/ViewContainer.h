#pragma once

#include "Window.h"
#include "BorderView.h"
#include "FileView.h"

struct ViewContainer : public Window
{
private:
    Dimension d;
    BorderView* frame = nullptr;
    Window* view = nullptr;
    bool focused = false;

public:
    ViewContainer(Dimension d) {
        this->d = d;
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
        if(_view) _view->resize(get_view_bounds());
    }

    Window* get_view() { return view; }

    void draw() override {
        frame->draw(); // frame must be drawn before content so that content does not get overriden
        if (view) {
            view->draw();
        }
        else if (focused) { // when no content view and focused window, move cursor top left
            move(d.y + 1, d.x + 1);
        }
    }

    void resize(Dimension d)override {
        this->d = d;
        frame->resize(d);
        if (view) {
            view->resize(get_view_bounds());
        }
    }

    void focus() override {
        focused = true;
        frame->focus();
        if (view) view->focus();
    }

    void unfocus() override {
        focused = false;
        frame->unfocus();
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
};
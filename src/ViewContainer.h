#pragma once

#include "Window.h"
#include "BorderView.h"
#include "FileView.h"

struct ViewContainer : public Window
{
private:
    Dimension d;
    BorderView* frame = nullptr;
    FileView* view = nullptr;
    bool focused = false;

public:
    ViewContainer(Dimension d, File* file) {
        this->d = d;
        frame = new BorderView(d);
        if (file) {
            view = new FileView(file, get_view_bounds());
        }
    }

    ~ViewContainer() {
        delete frame;
        delete view;
    }

    FileView* get_file_view() {
        return view;
    }

    Dimension get_view_bounds() {
        return (d.width >= 2 && d.height >= 2) ?
            Dimension(d.x + 1, d.y + 1, d.width - 2, d.height - 2) : Dimension();
    }

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

    void hide() override {
        frame->hide();
        if (view) view->hide();
    }

    void show() override {
        frame->show();
        if (view) view->show();
    }
};
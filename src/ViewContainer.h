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
        view = new FileView(file, get_view_bounds());
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
        view->draw();
    }

    void resize(Dimension d)override {
        // if (d == this->d) { return; }
        this->d = d;
        frame->resize(d);
        view->resize(get_view_bounds());
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
        view->hide();
    }

    void show() override {
        frame->show();
        view->show();
    }
};
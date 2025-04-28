#pragma once
#include "Window.h"

struct TestContentView : public Window {

    TestContentView(Dimension bounds = Dimension()) {}

    bool draw_called = false;
    bool show_called = false;
    bool resize_called = false;

    void reset() {
        draw_called = false;
        show_called = false;
        resize_called = false;
    }

    void resize(Dimension bounds) {
        Window::resize(bounds);
        resize_called = true;
    }

    void draw() {
        draw_called = true;
    }

    void show() {
        show_called = true;
    }
};
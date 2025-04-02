#pragma once
#include "window/ContentWindow.h"

struct TestContentView : public ContentWindow {

    TestContentView(Dimension bounds = Dimension()): ContentWindow(bounds) {}

    bool draw_called = false;
    bool show_called = false;
    bool resize_called = false;

    void reset() {
        draw_called = false;
        show_called = false;
        resize_called = false;
    }

    bool resize(Dimension bounds) override {
        Window::resize(bounds);
        resize_called = true;
        return true;
    }

    void draw() override {
        draw_called = true;
    }

    void show() override {
        show_called = true;
    }
};
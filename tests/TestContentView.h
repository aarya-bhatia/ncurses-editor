#pragma once
#include "window/ContentWindow.h"

struct TestContentView : public ContentWindow {

    TestContentView(Dimension bounds = Dimension()) : ContentWindow(bounds) {}

    bool draw_called = false;
    bool show_called = false;
    bool resize_called = false;

    std::shared_ptr<void> get_model()  override { return nullptr; }
    ContentType get_content_type() override { return ContentType::TestContent; }

    void reset() {
        draw_called = false;
        show_called = false;
        resize_called = false;
    }

    void resize(Dimension bounds) override {
        Window::resize(bounds);
        resize_called = true;
    }

    void draw() override {
        draw_called = true;
    }

    void show() override {
        show_called = true;
    }
};
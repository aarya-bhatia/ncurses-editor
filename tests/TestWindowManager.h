#pragma once
#include "window/IWindowManager.h"

struct TestWindowManager : public IWindowManager
{
    Dimension bounds;
    int draw_calls = 0;

    TestWindowManager(Dimension bounds) : bounds(bounds) {}

    Dimension get_bounds() const override { return bounds; }

    void draw()override {
        draw_calls++;
    }

    void show()override {}

    bool resize(Dimension bounds)override {
        this->bounds = bounds;
        return true;
    }

    ContentWindow* get_content_node()override {
        return nullptr;
    }

    void set_content(ContentWindow* content_window)override {}

    bool split_vertical(ContentWindow* new_content)override {
        return false;
    }

    bool split_horizontal(ContentWindow* new_content)override {
        return false;
    }

    void focus(ContentWindow* node)override {}
    void unfocus(ContentWindow* node)override {}
    ContentWindow* get_content_node_right(Window* current)override { return nullptr; }
    ContentWindow* get_content_node_left(Window* current)override { return nullptr; }
    ContentWindow* get_content_node_top(Window* current)override { return nullptr; }
    ContentWindow* get_content_node_bottom(Window* current) override { return nullptr; }

    int count_nodes() const override { return 0; }
    int count_content_nodes() const override { return 0; }
};

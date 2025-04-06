#pragma once
#include "window/IWindowManager.h"

struct TestWindowManager : public IWindowManager
{
    Dimension bounds;
    int draw_calls = 0;

    TestWindowManager(Dimension bounds) : bounds(bounds) {}

    Dimension get_bounds() const { return bounds; }

    void draw() {
        draw_calls++;
    }

    void show() {}

    bool resize(Dimension bounds) {
        this->bounds = bounds;
        return true;
    }

    ContentWindow* get_content_node() {
        return nullptr;
    }

    void set_content(ContentWindow* content_window) {}

    bool split_vertical(ContentWindow* new_content) {
        return false;
    }

    bool split_horizontal(ContentWindow* new_content) {
        return false;
    }
};

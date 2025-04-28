#pragma once
#include "Window.h"

struct TestWindow : public Window
{
    int id;
    Dimension bounds;

    TestWindow() {}

    File* get_file() { return nullptr; }

    Dimension get_bounds() { return bounds; }
    void resize(Dimension d) { bounds = d; }
    void draw() {}
    void redraw() {}
    void show() {}
    void clear() {}
    void focus() {}
    void unfocus() {}
};
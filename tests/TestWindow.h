#pragma once
#include "Window.h"

struct TestWindow
{
    int id;
    Dimension bounds;

    Dimension get_bounds() { return bounds; }
    void resize(Dimension d) { bounds = d; }
    void draw() {}
    void show() {}
    void clear() {}
    void focus() {}
    void unfocus() {}
};
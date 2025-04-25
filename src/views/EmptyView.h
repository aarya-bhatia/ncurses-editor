#pragma once
#include "Window.h"
#include "NcursesWindow.h"

struct EmptyView {

    NcursesWindow window;

    EmptyView(Dimension bounds) : window(bounds) {}

    void resize(Dimension bounds) {
        window = NcursesWindow(bounds);
    }

    void draw() {
        window.draw_border();
    }

    void show() {
        window.show();
    }
};
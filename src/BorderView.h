#pragma once

#include "Window.h"
#include "NcursesWindow.h"

struct BorderView : public Window {
    Dimension bounds;
    NcursesWindow frame;
    bool should_redraw = true;

    File* file = nullptr;

    BorderView(Dimension d) : bounds(d), frame(bounds) {}

    Dimension get_bounds() { return bounds; }

    void resize(Dimension d)
    {
        this->bounds = d;
        frame = NcursesWindow(bounds);
        should_redraw = true;
    }

    void draw()
    {
        if (should_redraw) {
            log_debug("drawing border window %s", bounds.debug_string().c_str());
            frame.draw_border();
            // TODO
            should_redraw = false;
        }
    }

    void show()
    {
        frame.show();
    }

    void focus() {
    }

    void unfocus() {
    }

    void clear() {
        frame.clear();
    }

    File* get_file() { return file; }

    void redraw() {
        should_redraw = true;
    }
};
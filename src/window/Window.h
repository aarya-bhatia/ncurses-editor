#pragma once

#include "Dimension.h"

struct ContainerWindow;
struct ContentWindow;

struct Window
{
    static const unsigned MIN_WINDOW_SIZE = 10;

    Dimension bounds;
    ContainerWindow* parent = nullptr;

    Window(Dimension bounds) : bounds(bounds) {}

    virtual ~Window() = default;

    Dimension get_bounds() const { return bounds; }

    virtual ContainerWindow* get_container() {
        return nullptr;
    }

    virtual ContentWindow* get_content() {
        return nullptr;
    }

    virtual void draw() = 0;
    virtual void show() = 0;

    virtual bool resize(Dimension bounds) {
        this->bounds = bounds;
        return true;
    }

    bool shrinkable(float ratio = 0.5) const {
        return bounds.width * ratio >= MIN_WINDOW_SIZE &&
            bounds.height * ratio >= MIN_WINDOW_SIZE;
    }
};

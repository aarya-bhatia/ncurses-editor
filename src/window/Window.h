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

    virtual void on_focus() {}
    virtual void on_unfocus() {}

    virtual bool resizable(Dimension bounds);
    virtual void resize(Dimension bounds);

    virtual ContentWindow* get_right_most_content_node() { return nullptr; }
    virtual ContentWindow* get_left_most_content_node() { return nullptr; }
    virtual ContentWindow* get_top_most_content_node() { return nullptr; }
    virtual ContentWindow* get_bottom_most_content_node() { return nullptr; }
};

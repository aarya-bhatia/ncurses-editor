#pragma once

#include "Dimension.h"
#include "log.h"

struct ContainerWindow;
struct ContentWindow;

struct Window
{
    int id = 0;
    static const unsigned MIN_WINDOW_SIZE = 6;

    Dimension bounds;
    ContainerWindow* parent = nullptr;

    Window(Dimension bounds) : bounds(bounds) {}

    virtual ~Window() = default;

    virtual std::string debug_string() const { return "window-" + std::to_string(id) + "-" + bounds.debug_string(); }

    Dimension get_bounds() const { return bounds; }

    bool is_legal() { return (get_container() || get_content()) && bounds.width >= MIN_WINDOW_SIZE && bounds.height >= MIN_WINDOW_SIZE && bounds.x >= 0 && bounds.y >= 0; }

    virtual ContainerWindow* get_container() {
        return nullptr;
    }

    virtual ContentWindow* get_content() {
        return nullptr;
    }

    virtual void draw() = 0;
    virtual void show() = 0;

    void focus() { log_debug("focus gained by %s", debug_string().c_str()); on_focus(); }
    void unfocus() { log_debug("focus lost by %s", debug_string().c_str()); on_unfocus(); }

    virtual bool resizable(Dimension bounds);
    virtual bool split_allowed() { return bounds.width / 2 >= MIN_WINDOW_SIZE && bounds.height / 2 >= MIN_WINDOW_SIZE; }

    void resize(Dimension bounds) {
        if (!resizable(bounds)) { return; }
        this->bounds = bounds;
        on_resize();
    }

    virtual bool is_presentable() { return this->get_content() != nullptr; }
    virtual ContentWindow* get_right_presentable_node() { return nullptr; }
    virtual ContentWindow* get_left_presentable_node() { return nullptr; }
    virtual ContentWindow* get_top_presentable_node() { return nullptr; }
    virtual ContentWindow* get_bottom_presentable_node() { return nullptr; }

protected:
    virtual void on_resize() = 0;
    virtual void on_focus() = 0;
    virtual void on_unfocus() = 0;
};

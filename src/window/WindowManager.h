#pragma once

#include "IWindowManager.h"
#include "Window.h"
#include "ContentWindow.h"
#include "ContainerWindow.h"
#include <assert.h>

struct WindowManager : public IWindowManager
{
    Dimension screen_bounds;
    Window* root_node = nullptr;
    Window* current_node = nullptr;

    WindowManager(Dimension bounds) : screen_bounds(bounds) {
    }

    ~WindowManager() {
        delete root_node;
        root_node = current_node = nullptr;
    }

    Dimension get_bounds() const override { return screen_bounds; }

    bool resize(Dimension bounds) override {
        if (root_node && root_node->resizable(bounds)) {
            root_node->resize(bounds);
            screen_bounds = bounds;
            return true;
        }

        return false;
    }

    void set_content(ContentWindow* content_window) override;
    ContentWindow* get_content_node() override;

    void draw() override {
        if (root_node) { root_node->draw(); }
    }

    void show() override {
        if (root_node) { root_node->show(); }
    }

    bool split_vertical(ContentWindow* new_content) override;
    bool split_horizontal(ContentWindow* new_content) override;

    ContentWindow* _find_content_node(Window* node);
    void _split(ContainerWindow* orig_parent, ContainerWindow* split_container, ContentWindow* new_content);
};
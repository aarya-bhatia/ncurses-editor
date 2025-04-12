#pragma once

#include "IWindowManager.h"
#include "Window.h"
#include "ContentWindow.h"
#include "ContainerWindow.h"
#include <assert.h>
#include "SequenceGenerator.h"

struct WindowManager : public IWindowManager
{
    Dimension screen_bounds;
    Window* root_node = nullptr;
    Window* current_node = nullptr;

    LinearSequenceGenerator<int> window_id_generator;

    WindowManager(Dimension bounds);
    ~WindowManager();

    Dimension get_bounds() const override { return screen_bounds; }

    void draw() override { if (root_node) { root_node->draw(); } }
    void show() override { if (root_node) { root_node->show(); } }

    bool resize(Dimension bounds) override;
    void focus(ContentWindow* node) override;
    void unfocus(ContentWindow* node) override;
    void set_content(ContentWindow* content_window) override;
    ContentWindow* get_content_node() override;
    bool split_vertical(ContentWindow* new_content) override;
    bool split_horizontal(ContentWindow* new_content) override;
    ContentWindow* _find_content_node(Window* node);
    void _split(ContainerWindow* orig_parent, ContainerWindow* split_container, ContentWindow* new_content);
    ContentWindow* get_content_node_right(Window* current) override;
    ContentWindow* get_content_node_left(Window* current) override;
    ContentWindow* get_content_node_top(Window* current) override;
    ContentWindow* get_content_node_bottom(Window* current) override;
};
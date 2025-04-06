#pragma once

#include "Window.h"
#include "ContentWindow.h"
#include "ContainerWindow.h"
#include <assert.h>

struct WindowManager
{
    Dimension screen_bounds;
    Window* root_node = nullptr;
    Window* current_node = nullptr;

    WindowManager(Dimension bounds);

    ~WindowManager();

    Dimension get_bounds() const;

    bool resize(Dimension bounds);

    void set_content(ContentWindow* content_window);

    ContentWindow* get_content_node();

    void draw();

    void show();

    ContentWindow* _find_content_node(Window* node);

    void _split(ContainerWindow* orig_parent, ContainerWindow* split_container, ContentWindow* new_content);
    bool split_vertical(ContentWindow* new_content);
    bool split_horizontal(ContentWindow* new_content);
};
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

    void set_content(ContentWindow* content_window);

    ContentWindow* get_content_node();

    void draw();

    void show();

    ContentWindow* _find_content_node(Window* node);

};
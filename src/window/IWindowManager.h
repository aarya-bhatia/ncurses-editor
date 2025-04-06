#pragma once
#include "Window.h"
#include "ContentWindow.h"

struct IWindowManager
{
    virtual ~IWindowManager() = default;
    virtual Dimension get_bounds() const = 0;
    virtual void draw() = 0;
    virtual void show() = 0;
    virtual bool resize(Dimension bounds) = 0;
    virtual ContentWindow* get_content_node() = 0;
    virtual void set_content(ContentWindow* content_window) = 0;
    virtual bool split_vertical(ContentWindow* new_content) = 0;
    virtual bool split_horizontal(ContentWindow* new_content) = 0;
};
#pragma once

#include "Types.h"
#include "File.h"

struct FileView;

struct Window
{
    virtual ~Window() = default;

    virtual Dimension get_bounds() = 0;
    virtual void focus() = 0;
    virtual void unfocus() = 0;
    virtual void draw() = 0;
    virtual void show() = 0;
    virtual void redraw() = 0;
    virtual void resize(Dimension d) = 0;

    virtual void partial_draw_character(Cursor position) {}
    virtual void partial_draw_line(Cursor position) {}
};
#pragma once

#include "Types.h"
#include "File.h"

struct Window
{
    virtual ~Window() = default;

    virtual File* get_file() = 0;
    virtual Dimension get_bounds() = 0;

    virtual void focus() = 0;
    virtual void unfocus() = 0;

    virtual void draw() = 0;
    virtual void show() = 0;
    virtual void clear() = 0;
    virtual void resize(Dimension d) = 0;
};

struct PartialDrawableWindow : public Window, IPartialDrawable {};
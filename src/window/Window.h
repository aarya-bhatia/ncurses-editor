#pragma once

#include "Dimension.h"
struct Window
{
    virtual ~Window() = default;
    virtual void draw() = 0;
    virtual void show() = 0;
    virtual void focus() = 0;
    virtual void unfocus() = 0;
    virtual void resize(Dimension d) = 0;
    virtual Window* copy(Dimension d) = 0;
};
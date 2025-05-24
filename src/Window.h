#pragma once

#include "Dimension.h"
#include "Cursor.h"

struct Window
{
    virtual ~Window() = default;

    virtual void draw() = 0;
    virtual void resize(Dimension d) = 0;
    virtual void focus() {}
    virtual void unfocus() {}
};
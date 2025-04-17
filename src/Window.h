#pragma once

#include "Dimension.h"
#include "File.h"
struct Window
{
    virtual ~Window() = default;
    virtual File *get_file() = 0;
    virtual void draw() = 0;
    virtual void show() = 0;
    virtual void focus() = 0;
    virtual void unfocus() = 0;
    virtual void resize(Dimension d) = 0;
    virtual Window* copy(Dimension d) = 0;
};
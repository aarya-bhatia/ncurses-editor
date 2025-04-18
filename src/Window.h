#pragma once

#include "Types.h"
#include "File.h"
struct Window : public IFocusable, IDrawable
{
    virtual ~Window() = default;
    virtual File* get_file() = 0;
    virtual Window* copy(Dimension d) = 0;
    virtual void clear() = 0;
    virtual Dimension get_bounds() = 0;
};
#pragma once

#include "Dimension.h"
#include "Cursor.h"

struct IDrawable {
    virtual ~IDrawable() = default;
    virtual void draw() = 0;
    virtual void show() = 0;
    virtual void resize(Dimension d) = 0;
};

struct IPartialDrawable : public IDrawable {
    virtual ~IPartialDrawable() = default;
    virtual void partial_draw_character(Cursor position) = 0;
    virtual void partial_draw_line(Cursor position) = 0;
    virtual void force_redraw() = 0;
};

struct IEventHandler {
    virtual ~IEventHandler() = default;
    virtual void handle_event(unsigned) = 0;
};

struct IEditor : public IEventHandler, IDrawable {
    virtual ~IEditor() = default;
};
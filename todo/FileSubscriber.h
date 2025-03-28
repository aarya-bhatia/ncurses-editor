#pragma once

#include "Cursor.h"
#include "Scroll.h"

struct FileSubscriber
{
    virtual ~FileSubscriber() = default;

    virtual void line_changed(int y) = 0;
    virtual void line_added(int y) = 0;
    virtual void line_removed(int y) = 0;

    virtual void cursor_changed(Cursor cur_prev, Cursor cur_new) = 0;
    virtual void scroll_changed(Scroll scroll_prev, Scroll scroll_new) = 0;

    virtual void file_loaded() = 0;
    virtual void file_closed() = 0;
};

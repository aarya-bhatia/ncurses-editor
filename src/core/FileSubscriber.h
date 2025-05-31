#pragma once

#include "Cursor.h"

struct FileSubscriber
{
    virtual ~FileSubscriber() = default;

    virtual void file_changed() {}
    virtual void line_added(Cursor) {}
    virtual void line_removed(Cursor) {}
    virtual void character_added(Cursor) {}
    virtual void character_removed(Cursor) {}
};

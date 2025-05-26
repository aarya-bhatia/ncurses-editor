#pragma once

#include "Cursor.h"

struct FileSubscriber
{
    virtual ~FileSubscriber() = default;

    virtual void file_changed() = 0;
    virtual void line_added(Cursor) = 0;
    virtual void line_removed(Cursor) = 0;
    virtual void character_added(Cursor) = 0;
    virtual void character_removed(Cursor) = 0;
};

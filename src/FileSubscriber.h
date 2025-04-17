#pragma once

#include "Cursor.h"
#include "Scroll.h"

struct File;
struct FileSubscriber
{
    virtual ~FileSubscriber() = default;
    virtual void on_file_reload(File& file) = 0;
    virtual void on_insert_character(File& file, Cursor position, char c) = 0;
    virtual void on_erase_character(File& file, Cursor position) = 0;
    virtual void on_replace_character(File& file, Cursor position) = 0;
};

#pragma once

#include <string>
#include "File.h"

struct Painter
{
    File* file = nullptr;

    Painter(File* file) : file(file) {}
    ~Painter();

    void draw_line(int y, const std::string& content);
    void insert_character(int y, int x, char c);
    void replace_character(int y, int x, char c);
    void erase_line(int y);
    void clear();
    void clear_line(int y);
    void clear_till_eol(int y, int x);
    void move(int y, int x);

private:
    bool line_visible(int line)
    {
        // return file->getDisplayY(line) >= 0 && line < file->getMaxY();
        return false;
    }

    void show_cursor()
    {
        // if (file->adjustScroll()) {
        //     RedrawFile();
        // }

        // wmove(file->window, file->getDisplayY(file->cursor.y), file->getDisplayX(file->cursor.x));
    }
};
#pragma once
#include "log.h"
#include <vector>
#include <list>
#include "Cursor.h"
#include "Scroll.h"

typedef unsigned int FileID;

struct File
{
    FileID id;
    char* filename;
    std::list<std::list<char>> lines;
    Cursor cursor;
    Scroll scroll;

    File(FileID id, const char* filename = nullptr)
    {
        this->id = id;
        this->filename = strdup(filename);
        this->lines.push_back({});
        this->cursor.x = 0;
        this->cursor.y = 0;
        this->cursor.line = this->lines.begin();
        this->cursor.col = this->lines.front().begin();
        this->scroll.dy = 0;
        this->scroll.dx = 0;
    }

    ~File()
    {
        free(this->filename);
        this->filename = nullptr;
    }

    FileID get_id() const { return this->id; }

    void set_filename(const char* filename)
    {
        free(this->filename);
        this->filename = strdup(filename);
    }

    const char* get_filename()
    {
        return this->filename;
    }

    bool is_named()
    {
        return this->filename != nullptr;
    }

    int load_file();
    int save_file();

    void move_cursor_y(int dy)
    {
        cursor.y += dy;

        if (dy > 0) {
            cursor.line = std::next(cursor.line, dy);
            cursor.col = cursor.line->begin();
        }
        else {
            cursor.line = std::prev(cursor.line, -dy);
            cursor.col = cursor.line->end();
        }
    }

    void move_cursor_x(int dx)
    {
        cursor.x += dx;

        if (dx > 0)
        {
            cursor.col = std::next(cursor.col, dx);
        }
        else {
            cursor.col = std::prev(cursor.col, -dx);
        }
    }

    int count_lines() const
    {
        return lines.size();
    }
};
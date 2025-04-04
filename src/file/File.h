#pragma once
#include <list>
#include "log.h"
#include <vector>
#include "Cursor.h"
#include "Scroll.h"
#include <ncurses.h>

typedef unsigned int FileID;

struct File
{
    FileID id;
    char* filename;
    std::list<std::list<char>> lines;
    Cursor cursor;
    Scroll scroll;
    WINDOW* window = nullptr;

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

    bool adjustScroll()
    {
        // adjust horizontal scroll
        if (cursor.x - scroll.dx < 0)
        {
            log_debug("scrolling left");
            scroll.dx = cursor.x;
            return true;
        }
        else if (cursor.x - scroll.dx >= getMaxX())
        {
            log_debug("scrolling right");
            scroll.dx = cursor.x - getMaxX() + 1;
            return true;
        }

        // adjust vertical scroll
        if (cursor.y - scroll.dy < 0)
        {
            log_debug("scrolling up");
            scroll.dy = cursor.y;
            return true;
        }
        else if (cursor.y - scroll.dy >= getMaxY())
        {
            log_debug("scrolling down");
            scroll.dy = cursor.y - getMaxY() + 1;
            return true;
        }

        return false;
    }

    void moveCursorY(int dy)
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

    void moveCursorX(int dx)
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

    void scrollY(int dy)
    {
        scroll.dy += dy;
    }

    void scrollX(int dx)
    {
        scroll.dx += dx;
    }

    int getScrollY() const
    {
        return scroll.dy;
    }

    int getScrollX() const
    {
        return scroll.dx;
    }

    int getCursorY() const
    {
        return cursor.y;
    }

    int getCursorX() const
    {
        return cursor.x;
    }

    int getRelativeY(int absy) const
    {
        return absy - getScrollY();
    }

    int getRelativeX(int absx) const
    {
        return absx - getScrollX();
    }

    int getDisplayY(int absy) const
    {
        return getRelativeY(absy) - getScrollY();
    }

    int getDisplayX(int absx) const
    {
        return getRelativeX(absx) - getScrollX();
    }

    int getMaxY() const
    {
        return getmaxy(window);
    }

    int getMaxX() const
    {
        return getmaxx(window);
    }

    int getNumLines() const
    {
        return lines.size();
    }
};
#pragma once

#include <ncurses.h>
#include "File.h"

struct FileView
{
    File* file = NULL;
    WINDOW* window = NULL;
    Scroll scroll;

    FileView(File* file, int x, int y, int width, int height)
    {
        this->file = file;
        this->window = newwin(y, x, height, width);
    }

    ~FileView()
    {
        delwin(window);
        window = NULL;
    }

    void Draw(Dimension dim);

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

    bool adjustScroll()
    {
        Cursor& cursor = file->cursor;

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
    }

};
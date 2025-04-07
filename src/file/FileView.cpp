#include "FileView.h"

bool FileView::scroll_to_ensure_cursor_visible()
{
    Cursor& cursor = file->cursor;

    // adjust horizontal page_scroll
    if (cursor.x - page_scroll.dx < 0)
    {
        log_debug("page_scrolling left");
        page_scroll.dx = cursor.x;
        return true;
    }
    else if (cursor.x - page_scroll.dx >= width())
    {
        log_debug("page_scrolling right");
        page_scroll.dx = cursor.x - width() + 1;
        return true;
    }

    // adjust vertical page_scroll
    if (cursor.y - page_scroll.dy < 0)
    {
        log_debug("page_scrolling up");
        page_scroll.dy = cursor.y;
        return true;
    }
    else if (cursor.y - page_scroll.dy >= height())
    {
        log_debug("page_scrolling down");
        page_scroll.dy = cursor.y - height() + 1;
        return true;
    }

    return false;
}
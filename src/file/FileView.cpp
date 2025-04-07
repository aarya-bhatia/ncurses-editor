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

void FileView::draw() {
    if (!window.get()) {
        return;
    }

    wclear(window.get());

    auto line_itr = file->lines.begin();
    std::advance(line_itr, page_scroll.dy);
    int count_lines = 0;
    for (; line_itr != file->lines.end() && count_lines < height(); line_itr++, count_lines++)
    {
        wmove(window.get(), count_lines, 0);
        auto& line = *line_itr;
        auto col_itr = line.begin();
        std::advance(col_itr, page_scroll.dx);
        int count_cols = 0;
        for (; col_itr != line.end() && count_cols < width(); col_itr++, count_cols++)
        {
            waddch(window.get(), *col_itr);
        }
    }
}
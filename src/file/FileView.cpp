#include "FileView.h"
#include <assert.h>

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

    if (!redraw) { return; }

    window.clear();

    auto line_itr = file->lines.begin();
    std::advance(line_itr, page_scroll.dy);
    int count_lines = 0;
    for (; line_itr != file->lines.end() && count_lines < height(); line_itr++, count_lines++)
    {
        window.move(count_lines, 0);
        auto& line = *line_itr;
        auto col_itr = line.begin();
        std::advance(col_itr, page_scroll.dx);
        int count_cols = 0;
        for (; col_itr != line.end() && count_cols < width(); col_itr++, count_cols++)
        {
            window.draw_character(count_lines, count_cols, *col_itr);
        }
    }

    redraw = false;
}

void FileView::partial_draw_character(Cursor position)
{
    int dpy = get_display_y(position.y);
    int dpx = get_display_x(position.x);
    if (!is_visible(dpy, dpx)) {
        return;
    }
    window.draw_character(dpy, dpx, *position.col);
}

void FileView::partial_draw_line(Cursor position)
{
    log_debug("partial redraw at Ln:%d Col:%d", position.y, position.x);

    window.clear_till_eol(position.y, position.x);
    auto col_itr = position.col;
    for (int x = position.x; x < position.line->size() && col_itr != position.line->end(); x++, col_itr++) {
        int dpy = get_display_y(position.y);
        int dpx = get_display_x(x);
        if (!is_visible(dpy, dpx)) {
            break;
        }
        char ch = *col_itr;
        window.draw_character(dpy, dpx, ch);
    }
}

void FileView::on_insert_character(File& file, Cursor position, char c)
{
    if (*this->file.get() != file) {
        return;
    }

    partial_draw_line(position);
}

void FileView::on_erase_character(File& file, Cursor position) {
    if (*this->file.get() != file) {
        return;
    }

    partial_draw_line(position);
}

void FileView::on_replace_character(File& file, Cursor position) {
    if (*this->file.get() != file) {
        return;
    }

    partial_draw_character(position);
}
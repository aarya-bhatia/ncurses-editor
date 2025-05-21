#include "FileView.h"
#include "log.h"
#include <assert.h>

FileView::FileView(File* file, Dimension bounds) : bounds(bounds), file(file)
{
    this->scroll.dx = 0;
    this->scroll.dy = 0;

    window = NcursesWindow(bounds);
    should_redraw = true;
}

bool FileView::scroll_to_ensure_cursor_visible()
{
    Cursor& cursor = file->cursor;

    if (cursor.x - this->scroll.dx < 0)
    {
        this->scroll.dx = cursor.x;
        return true;
    }
    else if (cursor.x - this->scroll.dx >= width())
    {
        this->scroll.dx = cursor.x - width() + 1;
        return true;
    }

    if (cursor.y - this->scroll.dy < 0)
    {
        this->scroll.dy = cursor.y;
        return true;
    }
    else if (cursor.y - this->scroll.dy >= height())
    {
        this->scroll.dy = cursor.y - height() + 1;
        return true;
    }

    return false;
}

void FileView::draw_content() {
    if (!window.get()) {
        return;
    }

    if (!should_redraw) { return; }
    redraw_count += 1;

    window.clear();

    auto line_itr = file->lines.begin();
    std::advance(line_itr, this->scroll.dy);
    int count_lines = 0;
    for (; line_itr != file->lines.end() && count_lines < height(); line_itr++, count_lines++)
    {
        window.move(count_lines, 0);
        auto& line = *line_itr;
        auto col_itr = line.begin();
        std::advance(col_itr, this->scroll.dx);
        int count_cols = 0;
        for (; col_itr != line.end() && count_cols < width(); col_itr++, count_cols++)
        {
            window.draw_character(count_lines, count_cols, *col_itr);
        }
    }

    should_redraw = false;
}

void FileView::draw() {
    // log_debug("drawing file view %s", bounds.debug_string().c_str());

    if (focused && scroll_to_ensure_cursor_visible()) {
        should_redraw = true;
    }

    draw_content();

    if (focused) {
        draw_cursor();
    }
}

void FileView::partial_draw_character(Cursor position)
{
    log_debug("partial redraw at Ln:%d Col:%d", position.y, position.x);

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

void FileView::draw_cursor()
{
    int cy = bounds.y + file->cursor.y - scroll.dy;
    int cx = bounds.x + file->cursor.x - scroll.dx;
    move(cy, cx);
}

void FileView::focus()
{
    log_debug("FileView::focus() %s", bounds.debug_string().c_str());
    focused = true;

    if (save_cursor_y < file->count_lines())
    {
        file->goto_line(save_cursor_y);
    }

    if (save_cursor_x < file->cursor.line->size()) {
        file->goto_column(save_cursor_x);
    }
}

void FileView::unfocus()
{
    log_debug("FileView::unfocus() %s", bounds.debug_string().c_str());
    focused = false;

    save_cursor_x = file->cursor.x;
    save_cursor_y = file->cursor.y;
}

void FileView::resize(Dimension d) {
    if (bounds == d) { return; }
    bounds = d;
    window = NcursesWindow(d);
    should_redraw = true;
}
#include "FileView.h"
#include "log.h"
#include <assert.h>

FileView::FileView(File* file, Dimension bounds) : bounds(bounds), file(file)
{
    this->scroll.dx = 0;
    this->scroll.dy = 0;

    window = NcursesWindow(bounds);
    redraw = true;
}

bool FileView::scroll_to_ensure_cursor_visible()
{
    Cursor& cursor = file->cursor;

    // adjust horizontal this->scroll
    if (cursor.x - this->scroll.dx < 0)
    {
        // log_debug("this->scrolling left");
        this->scroll.dx = cursor.x;
        return true;
    }
    else if (cursor.x - this->scroll.dx >= width())
    {
        // log_debug("this->scrolling right");
        this->scroll.dx = cursor.x - width() + 1;
        return true;
    }

    // adjust vertical this->scroll
    if (cursor.y - this->scroll.dy < 0)
    {
        // log_debug("this->scrolling up");
        this->scroll.dy = cursor.y;
        return true;
    }
    else if (cursor.y - this->scroll.dy >= height())
    {
        // log_debug("this->scrolling down");
        this->scroll.dy = cursor.y - height() + 1;
        return true;
    }

    return false;
}

void FileView::draw_content() {
    if (!window.get()) {
        return;
    }

    if (!redraw) { return; }

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

    redraw = false;
}

void FileView::draw() {
    if (scroll_to_ensure_cursor_visible()) {
        redraw = true;
    }

    draw_content();
    draw_cursor();
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

void FileView::draw_cursor()
{
    int cy = get_display_y(file->cursor.y);
    int cx = get_display_x(file->cursor.x);
    assert(cy >= 0 && cy < height());
    assert(cx >= 0 && cx < width());
    log_debug("drawing cursor at Ln %d Col %d", cy, cx);
    move(get_absolute_y(cy), get_absolute_x(cx));
}

// void FileView::on_focus()
// {
//     ContentWindow::on_focus();

//     if (save_cursor_y < file->count_lines())
//     {
//         file->goto_line(save_cursor_y);
//     }

//     if (save_cursor_x < file->cursor.line->size()) {
//         file->goto_column(save_cursor_x);
//     }
// }

// void FileView::on_unfocus()
// {
//     ContentWindow::on_unfocus();

//     save_cursor_x = file->cursor.x;
//     save_cursor_y = file->cursor.y;
// }

void FileView::resize(Dimension bounds) {
    window = NcursesWindow(bounds);
    redraw = true;
}
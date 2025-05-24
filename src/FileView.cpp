#include "FileView.h"
#include "log.h"
#include <assert.h>

FileView::FileView(File* f, Dimension d) : file(f)
{
    win = newwin(d.height, d.width, d.y, d.x);
}

bool FileView::scroll_to_ensure_cursor_visible()
{
    Cursor& cursor = file->cursor;

    if (cursor.x - scroll.dx < 0)
    {
        scroll.dx = cursor.x;
        return true;
    }
    else if (cursor.x - scroll.dx >= width())
    {
        scroll.dx = cursor.x - width() + 1;
        return true;
    }

    if (cursor.y - scroll.dy < 0)
    {
        scroll.dy = cursor.y;
        return true;
    }
    else if (cursor.y - scroll.dy >= height())
    {
        scroll.dy = cursor.y - height() + 1;
        return true;
    }

    return false;
}

void FileView::draw() {
    if (focused && scroll_to_ensure_cursor_visible()) {
        dirty = true;
    }

    // full render only when dirty
    if (dirty) {
        // dirty = false;

        // fill buffer with blanks
        werase(win);

        auto line_itr = file->lines.begin();
        std::advance(line_itr, scroll.dy);
        int count_lines = 0;

        for (; line_itr != file->lines.end() && count_lines < height(); line_itr++, count_lines++)
        {
            std::list<char>& line = *line_itr;
            std::list<char>::iterator col_itr = line.begin();
            std::advance(col_itr, scroll.dx);

            int count_cols = 0;

            for (; col_itr != line.end() && count_cols < width(); col_itr++, count_cols++)
            {
                mvwaddch(win, count_lines, count_cols, *col_itr);
            }
        }
    }

    // move the global cursor if this is the focused view
    if (focused) {
        // get absolute position of cursor
        int cy = getbegy(win) + file->cursor.y - scroll.dy;
        int cx = getbegx(win) + file->cursor.x - scroll.dx;

        move(cy, cx);
    }

    // stage changes
    wnoutrefresh(win);
}

void FileView::partial_draw_character(Cursor position)
{
    int dpy = position.y - scroll.dy;
    int dpx = position.x - scroll.dx;

    if (!is_visible(dpy, dpx)) {
        return;
    }

    mvwaddch(win, dpy, dpx, *position.col);
}

void FileView::partial_draw_line(Cursor position)
{
    wmove(win, position.y, position.x);
    wclrtoeol(win);

    auto col_itr = position.col;
    for (int x = position.x; x < position.line->size() && col_itr != position.line->end(); x++, col_itr++) {

        int dpy = position.y - scroll.dy;
        int dpx = x - scroll.dx;

        if (!is_visible(dpy, dpx)) {
            break;
        }

        char ch = *col_itr;
        mvwaddch(win, dpy, dpx, ch);
    }
}

void FileView::focus()
{
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
    focused = false;

    save_cursor_x = file->cursor.x;
    save_cursor_y = file->cursor.y;
}

void FileView::resize(Dimension d) {
    if (get_bounds() == d) {
        return;
    }

    log_debug("file view resized to %s", d.debug_string().c_str());

    // erase the contents on screen
    werase(win);
    wnoutrefresh(win);
    delwin(win);

    // new window at given position
    win = newwin(d.height, d.width, d.y, d.x);
    dirty = true;
}
#include "FileView.h"
#include "log.h"
#include <assert.h>

void FileView::_init(Dimension d)
{
    if (d.width > 0 && d.height > 0) {
        win = newwin(d.height, d.width, d.y, d.x);
    }
}

void FileView::_destroy() {
    if (!win) { return; }
    delwin(win);
    win = NULL;
}

FileView::FileView(File* f, Dimension d) : file(f)
{
    bounds = d;
    file->add_subscriber((FileSubscriber*)this);
    _init(d);
}

FileView::~FileView() {
    file->remove_subscriber((FileSubscriber*)this);
    _destroy();
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


void FileView::_draw_file_content(WINDOW* win)
{
    werase(win);

    auto line_itr = file->lines.begin();
    std::advance(line_itr, scroll.dy);

    int win_width, win_height;
    getmaxyx(win, win_height, win_width);

    int row = 0;
    while (line_itr != file->lines.end() && row < win_height) {
        auto col_itr = line_itr->begin();
        std::advance(col_itr, scroll.dx);

        int col = 0;
        while (col_itr != line_itr->end() && col < win_width) {
            mvwaddch(win, row, col, *col_itr);
            ++col_itr;
            ++col;
        }

        ++line_itr;
        ++row;
    }
}

void FileView::get_absolute_cursor(int& y, int& x)
{
    y = getbegy(win) + file->cursor.y - scroll.dy;
    x = getbegx(win) + file->cursor.x - scroll.dx;
}

void FileView::draw() {
    if (focused && scroll_to_ensure_cursor_visible()) {
        dirty = true;
    }

    // full render only when dirty
    if (dirty) {
        _draw_file_content(win);
    }

    dirty = false;

    // stage changes
    wnoutrefresh(win);
}

void FileView::_partial_draw_file(Cursor start)
{
    if (!is_visible(start.y - scroll.dy, 0)) {
        dirty = true; // needs to scroll and redraw
        return;
    }

    log_debug("partial file draw at %s", start.to_string().c_str());

    auto line_itr = start.line;
    int row = start.y - scroll.dy;

    while (line_itr != file->lines.end() && row < height())
    {
        auto col_itr = line_itr->begin();
        std::advance(col_itr, scroll.dx);

        wmove(win, row, 0);
        wclrtoeol(win);

        int col = 0;
        while (col_itr != line_itr->end() && col < width())
        {
            mvwaddch(win, row, col, *col_itr);
            ++col_itr;
            ++col;
        }

        ++line_itr;
        ++row;
    }

    while (row < height())
    {
        wmove(win, row, 0);
        wclrtoeol(win);
        ++row;
    }
}

void FileView::_partial_draw_character(Cursor position)
{
    int dpy = position.y - scroll.dy;
    int dpx = position.x - scroll.dx;

    if (!is_visible(dpy, dpx)) {
        return;
    }

    mvwaddch(win, dpy, dpx, *position.col);
}

void FileView::_partial_draw_line(Cursor position)
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
    if (bounds == d) {
        return;
    }

    bounds = d;
    log_debug("file view resized to %s", d.debug_string().c_str());

    // erase the contents on screen
    werase(win);
    wnoutrefresh(win);
    _destroy();

    // new window at given position
    _init(d);
    dirty = true;
}

void FileView::hide() {
    log_debug("hiding file view %s", bounds.debug_string().c_str());
    werase(win);
    wnoutrefresh(win);
}

void FileView::show() {
    log_debug("showing file view %s", bounds.debug_string().c_str());
    set_dirty();
}
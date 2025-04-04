#include "editor.h"
#include <curses.h>
#include <ncurses.h>
#include <string.h>
#include "log.h"
#include <sys/types.h>

void Editor::resize()
{
    delwin(edit_window);
    delwin(status_window);
    delwin(console_window);

    log_info("On resize(): No. lines: %d, cols: %d", LINES, COLS);

    edit_window = newwin(LINES - 2, COLS, 0, 0);
    status_window = newwin(1, COLS, LINES - 2, 0);
    console_window = newwin(1, COLS, LINES - 1, 0);

    refresh();

    force_redraw = true;
    update();
}

Editor::Editor()
{
    log_info("screen size: %dx%d", getmaxx(stdscr), getmaxy(stdscr));

    edit_window = newwin(LINES - 2, COLS, 0, 0);
    status_window = newwin(1, COLS, LINES - 2, 0);
    console_window = newwin(1, COLS, LINES - 1, 0);

    refresh();
    move(0, 0);
}

Editor::~Editor()
{
    delwin(edit_window);
    delwin(status_window);
    delwin(console_window);
}

void Editor::handle_event(unsigned c)
{
    if (c == CTRL_C)
    {
        quit = true;
        return;
    }
    else if (mode == INSERT_MODE)
    {
        handle_insert_mode_event(c);
    }
    else if (mode == NORMAL_MODE)
    {
        handle_normal_mode_event(c);
    }
    else if (mode == COMMAND_MODE)
    {
        handle_command_mode_event(c);
    }
}

void Editor::move_cursor_eol()
{
    File* file = file_manager.get_file();
    assert(file);
    Cursor& cursor = file->cursor;
    cursor.col = --(*cursor.line).end();
    std::list<char>& line_val = *cursor.line;
    cursor.x = line_val.empty() ? 0 : line_val.size() - 1;
}

void Editor::cursor_up()
{
    File* file = file_manager.get_file();
    assert(file);
    Cursor& cursor = file->cursor;

    if (cursor.y > 0)
    {
        --cursor.y;
        --cursor.line;

        if (cursor.x >= (*cursor.line).size())
        {
            move_cursor_eol();
        }
        else
        {
            cursor.col = (*cursor.line).begin();
            std::advance(cursor.col, cursor.x);
        }
    }
}

void Editor::cursor_down()
{
    File* file = file_manager.get_file();
    assert(file);
    auto& lines = file->lines;
    Cursor& cursor = file->cursor;

    if (cursor.y < lines.size() - 1)
    {
        ++cursor.y;
        ++cursor.line;

        std::list<char>& line_val = *cursor.line;
        if (cursor.x >= line_val.size())
        {
            cursor.x = line_val.empty() ? 0 : line_val.size() - 1;
        }

        cursor.col = line_val.begin();
        std::advance(cursor.col, cursor.x);
    }
}

void Editor::cursor_left()
{
    File* file = file_manager.get_file();
    assert(file);
    Cursor& cursor = file->cursor;

    if (cursor.x > 0)
    {
        --cursor.x;
        --cursor.col;
    }
}

void Editor::cursor_right()
{
    File* file = file_manager.get_file();
    assert(file);
    Cursor& cursor = file->cursor;
    std::list<char>& line_val = *cursor.line;

    if (line_val.size() > 0 && cursor.x < line_val.size() - 1)
    {
        ++cursor.x;
        ++cursor.col;
    }
}

void Editor::scroll_to_ensure_cursor_visible()
{
    File* file = file_manager.get_file();
    assert(file);
    Cursor& cursor = file->cursor;
    Scroll& scroll = file->scroll;

    // adjust horizontal scroll
    if (cursor.x - scroll.dx < 0)
    {
        log_debug("scrolling left");
        scroll.dx = cursor.x;
        force_redraw = true;
    }
    else if (cursor.x - scroll.dx >= getmaxx(edit_window))
    {
        log_debug("scrolling right");
        scroll.dx = cursor.x - getmaxx(edit_window) + 1;
        force_redraw = true;
    }

    // adjust vertical scroll
    if (cursor.y - scroll.dy < 0)
    {
        log_debug("scrolling up");
        scroll.dy = cursor.y;
        force_redraw = true;
    }
    else if (cursor.y - scroll.dy >= getmaxy(edit_window))
    {
        log_debug("scrolling down");
        scroll.dy = cursor.y - getmaxy(edit_window) + 1;
        force_redraw = true;
    }
}

void Editor::force_redraw_editor()
{
    log_debug("force redrawing");
    werase(edit_window);

    File* file = file_manager.get_file();
    if (!file)
    {
        return;
    }
    Scroll& scroll = file->scroll;
    auto& lines = file->lines;

    auto line_itr = lines.begin();
    std::advance(line_itr, scroll.dy);
    int count_lines = 0;
    int max_lines, max_cols;
    getmaxyx(edit_window, max_lines, max_cols);
    for (; line_itr != lines.end() && count_lines < max_lines; line_itr++, count_lines++)
    {
        wmove(edit_window, count_lines, 0);
        auto& line = *line_itr;
        auto col_itr = line.begin();
        std::advance(col_itr, scroll.dx);
        int count_cols = 0;
        for (; col_itr != line.end() && count_cols < max_cols; col_itr++, count_cols++)
        {
            waddch(edit_window, *col_itr);
        }
    }
}

void Editor::command(const std::string& command)
{
    if (command == "q")
    {
        quit = true;
    }
    else if (strncmp(command.c_str(), "open ", 5) == 0)
    {
        std::vector<std::string> filenames = splitwords(command.substr(5), " ");
        for (std::string& filename : filenames)
        {
            file_manager.open_file(filename.c_str());
            log_debug("File index: %d", file_manager.get_file_index());
        }
    }
    else if (command == "next")
    {
        file_manager.next_file();
        force_redraw = true;
        log_debug("File index: %d", file_manager.get_file_index());
    }
    else if (command == "prev")
    {
        file_manager.prev_file();
        force_redraw = true;
        log_debug("File index: %d", file_manager.get_file_index());
    }
    else
    {
        statusline = "no such command";
    }
}

void Editor::handle_insert_mode_event(unsigned c)
{
    File* file = file_manager.get_file();
    assert(file);
    std::list<char>& line_val = *file->cursor.line;

    switch (c)
    {
    case CTRL_ESCAPE:
        cursor_left();
        mode = NORMAL_MODE;
        break;

    default:
        if (PRINTABLE(c))
        {
            file->cursor.col = line_val.insert(file->cursor.col, c);
            file->cursor.col++;
            file->cursor.x++;
            file->dirty_lines.push_back(file->cursor);
        }
    }
}

void Editor::redraw_line(Cursor info)
{
    File* file = file_manager.get_file();
    if (!file)
    {
        return;
    }
    Scroll& scroll = file->scroll;

    int display_line = info.y - scroll.dy;
    if (display_line < 0 || display_line >= getmaxy(edit_window))
    {
        return;
    }

    wmove(edit_window, display_line, 0);
    wclrtoeol(edit_window);
    int max_cols = getmaxx(edit_window);

    std::list<char>& line = *info.line;
    auto col_itr = line.begin();
    std::advance(col_itr, scroll.dx);
    int count_cols = 0;
    for (; col_itr != line.end() && count_cols < max_cols; col_itr++, count_cols++)
    {
        waddch(edit_window, *col_itr);
    }
}

void Editor::handle_normal_mode_event(unsigned c)
{
    File* file = file_manager.get_file();

    switch (c)
    {
    case 'h':
        if (file)
        {
            cursor_left();
        }
        break;

    case 'l':
        if (file)
        {
            cursor_right();
        }
        break;

    case 'j':
        if (file)
        {
            cursor_down();
        }
        break;

    case 'k':
        if (file)
        {
            cursor_up();
        }
        break;

    case CTRL_ESCAPE:
        statusline = "";
        break;

    case '0':
        if (file)
        {
            file->cursor.x = 0;
            file->cursor.col = (*file->cursor.line).begin();
        }
        break;

    case '$':
        if (file)
        {
            move_cursor_eol();
        }
        break;

    case 'i':
        if (file)
        {
            mode = INSERT_MODE;
        }
        break;

    case ':':
        mode = COMMAND_MODE;
        mode_line = "";
        break;
    }
}

void Editor::handle_command_mode_event(unsigned c)
{
    switch (c)
    {
    case CTRL_ENTER:
        mode = NORMAL_MODE;
        command(mode_line);
        break;

    default:
        if (PRINTABLE(c))
        {
            mode_line += c;
        }
    }
}

void Editor::update()
{
    File* file = file_manager.get_file();
    if (!file)
    {
        draw();
        return;
    }

    auto& dirty_lines = file->dirty_lines;

    scroll_to_ensure_cursor_visible();
    if (force_redraw)
    {
        force_redraw = false;
        dirty_lines.clear();
        force_redraw_editor();
    }

    for (Cursor& line : dirty_lines)
    {
        redraw_line(line);
    }

    dirty_lines.clear();
    draw();
}

void join_left_and_right_status(int ncols, char* left_status, char* right_status, char* full_status)
{
    for (int i = 0; i < ncols; i++)
    {
        full_status[i] = ' ';
    }

    int i = 0;
    for (; i < strlen(left_status); i++)
    {
        full_status[i] = left_status[i];
    }

    if (i >= ncols)
    {
        return;
    }

    int max_right_len = ncols - i;
    int right_len = strlen(right_status);
    if (max_right_len < right_len)
    {
        right_status[max_right_len] = 0;
        right_len = max_right_len;
    }

    if (right_len == 0)
    {
        return;
    }

    int between_len = ncols - i - right_len;
    int start_right = i + between_len;
    for (int j = start_right; j < ncols; j++)
    {
        full_status[j] = right_status[j - start_right];
    }
}

void Editor::draw()
{
    wrefresh(edit_window);

    int ncols = getmaxx(status_window);
    char* left_status = new char[ncols + 1];
    char* right_status = new char[ncols + 1];
    char* full_status = new char[ncols + 1];

    auto mode_name = mode_names.find(mode);
    assert(mode_name != mode_names.end());

    snprintf(left_status, ncols, "-- %s --", (*mode_name).second);

    File* file = file_manager.get_file();
    if (file)
    {
        snprintf(right_status, ncols, "%s | Ln:%d Col:%d", file->filename, file->cursor.y, file->cursor.x);
    }
    else
    {
        snprintf(right_status, ncols, "no file | Ln:%d Col:%d", 0, 0);
    }

    join_left_and_right_status(ncols, left_status, right_status, full_status);

    mvwprintw(status_window, 0, 0, full_status);
    wrefresh(status_window);

    delete[] left_status;
    delete[] right_status;
    delete[] full_status;

    werase(console_window);
    ncols = getmaxx(console_window);
    if (mode == COMMAND_MODE)
    {
        std::string tmp = ":" + mode_line;
        if (tmp.size() >= ncols)
        {
            tmp = tmp.substr(tmp.size() - ncols - 1);
        }
        mvwprintw(console_window, 0, 0, tmp.substr(0, ncols).c_str());
    }
    else
    {
        mvwprintw(console_window, 0, 0, statusline.substr(0, ncols).c_str());
    }

    wrefresh(console_window);

    if (!file)
    {
        move(0, 0);
        return;
    }

    Scroll& scroll = file->scroll;
    Cursor& cursor = file->cursor;

    int cy = cursor.y - scroll.dy;
    int cx = cursor.x - scroll.dx;
    if (cy < 0 || cy >= getmaxy(edit_window))
    {
        cy = 0;
        log_warn("illegal cursor or scroll value");
    }
    if (cx < 0 || cx >= getmaxx(edit_window))
    {
        cx = 0;
        log_warn("illegal cursor or scroll value");
    }
    log_debug("display cursor: y:%d x:%d", cy, cx);
    move(cy, cx);
}

void Editor::open(const std::vector<std::string>& filenames)
{
    for (const std::string& filename : filenames)
    {
        file_manager.open_file(filename.c_str());
    }

    if (file_manager.get_file())
    {
        force_redraw = true;
        update();
    }
}

void Editor::close(const std::string& filename)
{
    // TODO: close file directly
    if (file_manager.open_file(filename.c_str()))
    {
        file_manager.close_file();
    }
}
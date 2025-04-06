#include "editor.h"
#include <curses.h>
#include <ncurses.h>
#include <string.h>
#include "log.h"
#include <sys/types.h>

Editor::Editor() : window_manager(Dimension(0, 0, COLS, LINES - 2))
{
    log_info("screen size: %dx%d", getmaxx(stdscr), getmaxy(stdscr));

    status_window = newwin(1, COLS, LINES - 2, 0);
    console_window = newwin(1, COLS, LINES - 1, 0);

    refresh();
    move(0, 0);
}

Editor::~Editor()
{
    delwin(status_window);
    delwin(console_window);
}

void Editor::resize()
{
    if (!window_manager.resize(Dimension(0, 0, COLS, LINES - 2))) {
        log_warn("resize failed");
        return;
    }

    log_info("resize(): lines:%d cols:%d", LINES, COLS);

    delwin(status_window);
    delwin(console_window);

    status_window = newwin(1, COLS, LINES - 2, 0);
    console_window = newwin(1, COLS, LINES - 1, 0);

    refresh();

    force_redraw = true;
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

FileView* Editor::get_current_view()
{
    ContentWindow* window = window_manager.get_content_node();
    if (!window) {
        return nullptr;
    }

    return dynamic_cast<FileView*>(window);
}


std::shared_ptr<File> Editor::get_current_file()
{
    FileView* file_view = get_current_view();
    assert(file_view);
    return file_view->file;
}

void Editor::move_cursor_eol()
{
    auto file = get_current_file();
    assert(file);
    Cursor& cursor = file->cursor;
    cursor.col = --(*cursor.line).end();
    std::list<char>& line_val = *cursor.line;
    cursor.x = line_val.empty() ? 0 : line_val.size() - 1;
}

void Editor::cursor_up()
{
    auto file = get_current_file();
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
    auto file = get_current_file();
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
    auto file = get_current_file();
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
    auto file = get_current_file();
    assert(file);
    Cursor& cursor = file->cursor;
    std::list<char>& line_val = *cursor.line;

    if (line_val.size() > 0 && cursor.x < line_val.size() - 1)
    {
        ++cursor.x;
        ++cursor.col;
    }
}

void Editor::force_redraw_editor()
{
    log_debug("force redrawing");

    auto file_view = get_current_view();
    if (!file_view) {
        return;
    }

    file_view->draw();

    // werase(edit_window);

    // if (!file)
    // {
    //     return;
    // }
    // Scroll& scroll = file->scroll;
    // auto& lines = file->lines;

    // auto line_itr = lines.begin();
    // std::advance(line_itr, scroll.dy);
    // int count_lines = 0;
    // int max_lines, max_cols;
    // getmaxyx(edit_window, max_lines, max_cols);
    // for (; line_itr != lines.end() && count_lines < max_lines; line_itr++, count_lines++)
    // {
    //     wmove(edit_window, count_lines, 0);
    //     auto& line = *line_itr;
    //     auto col_itr = line.begin();
    //     std::advance(col_itr, scroll.dx);
    //     int count_cols = 0;
    //     for (; col_itr != line.end() && count_cols < max_cols; col_itr++, count_cols++)
    //     {
    //         waddch(edit_window, *col_itr);
    //     }
    // }
}

void Editor::command(const std::string& command)
{
    FileView* file_view = get_current_view();
    std::shared_ptr<File> file = nullptr;
    if (file_view) {
        file = file_view->file;
    }

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
        }
    }
    else if (command == "next")
    {
        if (!file) {
            return;
        }
        open(file_manager.next_file(file));
        force_redraw = true;
    }
    else if (command == "prev")
    {
        open(file_manager.prev_file(file));
        force_redraw = true;
    }
    else
    {
        statusline = "no such command";
    }
}

void Editor::handle_insert_mode_event(unsigned c)
{
    auto file = get_current_file();
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
            // file->dirty_lines.push_back(file->cursor);
        }
    }
}

void Editor::redraw_line(Cursor info)
{
    // auto file = get_current_file();
    // if (!file)
    // {
    //     return;
    // }
    // Scroll& scroll = file->scroll;

    // int display_line = info.y - scroll.dy;
    // if (display_line < 0 || display_line >= getmaxy(edit_window))
    // {
    //     return;
    // }

    // wmove(edit_window, display_line, 0);
    // wclrtoeol(edit_window);
    // int max_cols = getmaxx(edit_window);

    // std::list<char>& line = *info.line;
    // auto col_itr = line.begin();
    // std::advance(col_itr, scroll.dx);
    // int count_cols = 0;
    // for (; col_itr != line.end() && count_cols < max_cols; col_itr++, count_cols++)
    // {
    //     waddch(edit_window, *col_itr);
    // }
}

void Editor::handle_normal_mode_event(unsigned c)
{
    auto file = get_current_file();

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
    auto file = get_current_file();
    if (!file)
    {
        draw();
        return;
    }

    // auto& dirty_lines = file->dirty_lines;

    // scroll_to_ensure_cursor_visible();
    // if (force_redraw)
    // {
    //     force_redraw = false;
    //     dirty_lines.clear();
    //     force_redraw_editor();
    // }

    // for (Cursor& line : dirty_lines)
    // {
    //     redraw_line(line);
    // }

    // dirty_lines.clear();
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

void Editor::show() { window_manager.show(); }

void Editor::draw()
{
    window_manager.draw();

    int ncols = getmaxx(status_window);
    char* left_status = new char[ncols + 1];
    char* right_status = new char[ncols + 1];
    char* full_status = new char[ncols + 1];

    auto mode_name = mode_names.find(mode);
    assert(mode_name != mode_names.end());

    snprintf(left_status, ncols, "-- %s --", (*mode_name).second);

    auto view = dynamic_cast<FileView*> (window_manager.get_content_node()->get_content());
    std::shared_ptr<File> file;
    if (view) {
        file = view->file;
    }
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
    if (cy < 0 || cy >= view->height())
    {
        cy = 0;
        log_warn("illegal cursor or scroll value");
    }
    if (cx < 0 || cx >= view->width())
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
        auto file = file_manager.open_file(filename.c_str());
        auto file_view = file_manager.get_file_view(file);
        window_manager.set_content(file_view);
    }

    force_redraw = true;
}

void Editor::open(std::shared_ptr<File> file)
{
    auto view = file_manager.get_file_view(file);
    if (!view) {
        return;
    }

    window_manager.set_content(view);
    force_redraw = true;
}
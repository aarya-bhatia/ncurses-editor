#include "editor.h"
#include <curses.h>
#include <ncurses.h>
#include <string.h>
#include "log.h"
#include <sys/types.h>
#include "window/WindowManager.h"

Editor::Editor()
{
    this->window_manager = std::shared_ptr<IWindowManager>(new WindowManager(Dimension(0, 0, COLS, LINES - 2)));
    this->file_manager = std::unique_ptr<FileManager>(new FileManager(this->window_manager));

    log_info("screen size: %dx%d", getmaxx(stdscr), getmaxy(stdscr));

    status_window = std::unique_ptr<StatusWindow>(new StatusWindow(*this, Dimension(0, LINES - 2, COLS, 1)));
    console_window = NcursesWindow(Dimension(0, LINES - 1, COLS, 1));

    refresh();
    move(0, 0);
}

void Editor::resize()
{
    if (!window_manager->resize(Dimension(0, 0, COLS, LINES - 2))) {
        log_warn("resize failed");
        return;
    }

    log_info("resize(): lines:%d cols:%d", LINES, COLS);

    status_window = std::unique_ptr<StatusWindow>(new StatusWindow(*this, Dimension(0, LINES - 2, COLS, 1)));
    console_window = NcursesWindow(Dimension(0, LINES - 1, COLS, 1));

    refresh();
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
    ContentWindow* window = window_manager->get_content_node();
    if (!window) {
        return nullptr;
    }

    if (window->get_content_type() == ContentWindow::ContentType::FileContent) {
        return static_cast<FileView*>(window);
    }

    return nullptr;
}


std::shared_ptr<File> Editor::get_current_file()
{
    FileView* file_view = get_current_view();
    if (!file_view) {
        return nullptr;
    }

    return std::static_pointer_cast<File>(file_view->get_model());
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
            file_manager->open_file(filename.c_str());
        }
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

    if (!file) {
        switch (c) {
        case CTRL_ESCAPE:
            statusline = "";
            break;
        case ':':
            mode = COMMAND_MODE;
            mode_line = "";
            break;
        }
    }
    else {
        assert(file);

        switch (c)
        {
        case 'h':
            cursor_left();
            break;

        case 'l':
            cursor_right();
            break;

        case 'j':
            cursor_down();
            break;

        case 'k':
            cursor_up();
            break;

        case CTRL_ESCAPE:
            statusline = "";
            break;

        case '0':
            file->cursor.x = 0;
            file->cursor.col = (*file->cursor.line).begin();
            break;

        case '$':
            move_cursor_eol();
            break;

        case 'i':
            mode = INSERT_MODE;
            break;

        case ':':
            mode = COMMAND_MODE;
            mode_line = "";
            break;
        }
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


void Editor::show() {
    window_manager->show();
    status_window->show();
    wrefresh(console_window.get());
}

void Editor::draw()
{
    window_manager->draw();
    status_window->draw();

    wclear(console_window.get());
    wprintw(console_window.get(), "Console Window");

    // werase(console_window);
    // ncols = getmaxx(console_window);
    // if (mode == COMMAND_MODE)
    // {
    //     std::string tmp = ":" + mode_line;
    //     if (tmp.size() >= ncols)
    //     {
    //         tmp = tmp.substr(tmp.size() - ncols - 1);
    //     }
    //     mvwprintw(console_window, 0, 0, tmp.substr(0, ncols).c_str());
    // }
    // else
    // {
    //     mvwprintw(console_window, 0, 0, statusline.substr(0, ncols).c_str());
    // }

    // wrefresh(console_window);

    // if (!file)
    // {
    //     move(0, 0);
    //     return;
    // }

    FileView* view = get_current_view();
    std::shared_ptr<File> file = get_current_file();
    if (!file) {
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
        file_manager->open_in_current_window(file_manager->open_file(filename.c_str()));
    }
}
#include "editor.h"
#include <curses.h>
#include <ncurses.h>
#include <string.h>
#include "log.h"
#include <sys/types.h>
#include "window/WindowManager.h"
#include "file/FileView.h"
#include "file/FileSubscriber.h"

Editor::Editor()
{
    log_info("screen size: %dx%d", getmaxx(stdscr), getmaxy(stdscr));

    this->window_manager = std::shared_ptr<IWindowManager>(new WindowManager(Dimension(0, 0, COLS, LINES - 2)));
    this->file_manager = std::unique_ptr<FileManager>(new FileManager(this->window_manager));
    this->status_window = std::unique_ptr<StatusWindow>(new StatusWindow(*this, Dimension(0, LINES - 2, COLS, 1)));
    this->console_window = std::unique_ptr<ConsoleWindow>(new ConsoleWindow(*this, Dimension(0, LINES - 1, COLS, 1)));

    refresh();
}

void Editor::resize()
{
    if (!window_manager->resize(Dimension(0, 0, COLS, LINES - 2))) {
        log_warn("resize failed");
        return;
    }

    log_info("resize(): lines:%d cols:%d", LINES, COLS);
    status_window->resize(Dimension(0, LINES - 2, COLS, 1));
    console_window->resize(Dimension(0, LINES - 1, COLS, 1));
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


void Editor::command(const std::string& command)
{
    FileView* file_view = get_current_view();
    std::shared_ptr<File> file = get_current_file();
    log_debug("Got command: %s", command.c_str());

    if (command == "q" || command == "quit")
    {
        quit = true;
    }
    else if (strncmp(command.c_str(), "open ", 5) == 0)
    {
        std::vector<std::string> filenames = splitwords(command.substr(5), " ");
        this->open(filenames);
    }
    else if (strncmp(command.c_str(), "save", 5) == 0)
    {
        if (file) {
            file->save_file();
        }
    }
    else if (!strncmp(command.c_str(), "sp", 2) || !strncmp(command.c_str(), "split", 5))
    {
        if (file_view) {
            window_manager->split_horizontal(new FileView(*file_view));
        }
    }
    else if (!strncmp(command.c_str(), "vs", 2) || !strncmp(command.c_str(), "vsplit", 6))
    {
        if (file_view) {
            window_manager->split_vertical(new FileView(*file_view));
        }
    }
    else if (is_number(command))
    {
        if (file) {
            file->goto_line(atoi(command.c_str()));
        }
    }
    else
    {
        statusline = "no such command: " + command;
    }
}

void Editor::handle_insert_mode_event(unsigned c)
{
    auto file = get_current_file();

    switch (c)
    {
    case CTRL_ESCAPE:
        if (file) {
            file->cursor_left();
        }
        mode = NORMAL_MODE;
        break;

    default:
        if (PRINTABLE(c))
        {
            file->insert_character(c);
        }
    }
}

void Editor::handle_normal_mode_event(unsigned c)
{
    auto file = get_current_file();

    if (!file) {
        switch (c) {
        case CTRL_ESCAPE:
            statusline = "";
            return;
        case ':':
            mode = COMMAND_MODE;
            mode_line = "";
            return;
        default:
            return;
        }
    }
    else {
        assert(file);

        switch (c)
        {
        case 'h':
            file->cursor_left();
            return;

        case 'l':
            file->cursor_right();
            return;

        case 'j':
            file->cursor_down();
            return;

        case 'k':
            file->cursor_up();
            return;

        case '0':
            file->move_begin();
            return;

        case '$':
            file->move_cursor_eol();
            return;

        case 'G':
            file->move_cursor_eof();
            return;

        case 'g':
            file->goto_line(0);
            return;

        case 'i':
            mode = INSERT_MODE;
            return;

        case CTRL_ESCAPE:
            statusline = "";
            return;

        case ':':
            mode = COMMAND_MODE;
            mode_line = "";
            return;
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

    case CTRL_BACKSPACE:
    case CTRL_DEL:
        if (!mode_line.empty())
        {
            mode_line = mode_line.substr(0, mode_line.size() - 1);
        }
        break;

    default:
        if (PRINTABLE(c))
        {
            mode_line += c;
        }
        break;
    }
}


void Editor::show() {
    window_manager->show();
    status_window->show();
    console_window->show();
}

void Editor::draw()
{
    FileView* view = get_current_view();

    window_manager->draw();
    status_window->draw();
    console_window->draw();

    std::shared_ptr<File> file = get_current_file();
    if (!file) {
        move(0, 0);
        return;
    }

    int cy = view->get_display_y(file->cursor.y);
    int cx = view->get_display_x(file->cursor.x);

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

    move(cy, cx);
}

void Editor::open(const std::vector<std::string>& filenames)
{
    for (const std::string& filename : filenames)
    {
        file_manager->open_in_current_window(file_manager->open_file(filename.c_str()));
    }
}
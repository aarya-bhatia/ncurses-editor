#include "editor.h"
#include <curses.h>
#include <ncurses.h>
#include <string.h>
#include "log.h"
#include <sys/types.h>
#include "window/WindowManager.h"

Editor::Editor()
{
    log_info("screen size: %dx%d", getmaxx(stdscr), getmaxy(stdscr));

    this->window_manager = std::shared_ptr<IWindowManager>(new WindowManager(Dimension(0, 0, COLS, LINES - 2)));
    this->file_manager = std::unique_ptr<FileManager>(new FileManager(this->window_manager));
    this->status_window = std::unique_ptr<StatusWindow>(new StatusWindow(*this, Dimension(0, LINES - 2, COLS, 1)));
    this->console_window = std::unique_ptr<ConsoleWindow>(new ConsoleWindow(*this, Dimension(0, LINES - 1, COLS, 1)));

    refresh();

    auto new_file = this->file_manager->open_untitled_file();
    this->file_manager->open_in_current_window(new_file);
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
        this->open(filenames);
    }
    else
    {
        statusline = "no such command";
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
            file->cursor_left();
            break;

        case 'l':
            file->cursor_right();
            break;

        case 'j':
            file->cursor_down();
            break;

        case 'k':
            file->cursor_up();
            break;

        case '0':
            file->move_begin();
            break;

        case '$':
            file->move_cursor_eol();
            break;

        case 'i':
            mode = INSERT_MODE;
            break;

        case CTRL_ESCAPE:
            statusline = "";
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
    console_window->show();
}

void Editor::draw()
{
    FileView* view = get_current_view();
    if (view) {
        view->scroll_to_ensure_cursor_visible();
    }

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
    log_debug("file:%d:'%s' display cursor: y:%d x:%d", file->id, file->filename, cy, cx);
    move(cy, cx);
}

void Editor::open(const std::vector<std::string>& filenames)
{
    for (const std::string& filename : filenames)
    {
        file_manager->open_in_current_window(file_manager->open_file(filename.c_str()));
    }
}
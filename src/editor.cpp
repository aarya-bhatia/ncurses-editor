#include "editor.h"
#include <string.h>
#include "log.h"
#include "FileView.h"
#include "FileSubscriber.h"

Editor::Editor(): window_manager(Dimension(0, 0, COLS, LINES - 2))
{
    log_info("screen size: %dx%d", getmaxx(stdscr), getmaxy(stdscr));

    this->status_window = std::unique_ptr<StatusWindow>(new StatusWindow(*this, Dimension(0, LINES - 2, COLS, 1)));
    this->console_window = std::unique_ptr<ConsoleWindow>(new ConsoleWindow(*this, Dimension(0, LINES - 1, COLS, 1)));
    this->file_update_handler = std::unique_ptr<FileUpdateHandler>(new FileUpdateHandler(this->window_manager));
    this->file_manager = std::unique_ptr<FileManager>(new FileManager(*this->file_update_handler));

    refresh();
}

void Editor::resize()
{
    log_info("resize(): lines:%d cols:%d", LINES, COLS);
    window_manager.resize(Dimension(0, 0, COLS, LINES - 2));
    status_window->resize(Dimension(0, LINES - 2, COLS, 1));
    console_window->resize(Dimension(0, LINES - 1, COLS, 1));
    refresh();
}

void Editor::handle_event(unsigned c)
{
    if (quit) {
        log_info("event skipped as quit flag is on.");
        return;
    }

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

Window* Editor::get_current_view()
{
    WMNode* node = window_manager.current_node;
    return node ? node->get_current_tab_window() : nullptr;
}


File* Editor::get_current_file()
{
    Window* window = get_current_view();
    return window ? window->get_file() : nullptr;
}


void Editor::command(const std::string& command)
{
    File* file = get_current_file();
    log_debug("Got command: %s", command.c_str());

    if (command == "q" || command == "quit")
    {
        log_warn("quit flag set");
        quit = true;
    }
    else if (command.substr(0, 5) == "open ")
    {
        std::vector<std::string> filenames = splitwords(command.substr(5), " ");
        this->open(filenames);
    }
    else if (command == "save")
    {
        if (file) {
            file->save_file();
        }
    }
    else if (command == "sp" || command == "split")
    {
        window_manager.splith();
    }
    else if (command == "vs" || command == "vsplit")
    {
        window_manager.splitv();
    }
    else if (command == "right")
    {
        window_manager.focus_right();
    }
    else if (command == "left")
    {
        window_manager.focus_left();
    }
    else if (command == "top")
    {
        window_manager.focus_top();
    }
    else if (command == "bottom")
    {
        window_manager.focus_bottom();
    }
    else if (command == "next")
    {
        window_manager.current_node->open_next_tab();
    }
    else if (command == "prev")
    {
        window_manager.current_node->open_prev_tab();
    }
    else if (is_number(command))
    {
        if (file) {
            file->goto_line(atoi(command.c_str()));
        }
    }
    else
    {
        log_warn("no such command: %s", command.c_str());
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

void Editor::handle_normal_mode_two_key_seq()
{
    auto file = get_current_file();
    assert(file);
    assert(file->normal_mode_buffer.size() == 2);

    if (file->normal_mode_buffer == "gg") {
        file->goto_line(0);
    }

    file->normal_mode_buffer = "";
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

        if (file->normal_mode_buffer.size() > 0) {
            file->normal_mode_buffer += c;
            handle_normal_mode_two_key_seq();
            return;
        }

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
            file->normal_mode_buffer += c;
            return;

        case 'i':
            mode = INSERT_MODE;
            return;

        case CTRL_ESCAPE:
            statusline = "";
            file->normal_mode_buffer.clear();
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
        if (mode_line.empty())
        {
            mode = NORMAL_MODE;
            break;
        }
        else
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
    window_manager.show();
    status_window->show();
    console_window->show();
}

void Editor::draw()
{
    window_manager.draw();
    status_window->draw();
    console_window->draw();

    if(!window_manager.current_node || !window_manager.current_node->get_current_tab_window()) {
        move(0, 0);
    }
}

void Editor::open(const std::vector<std::string>& filenames)
{
    for (const std::string& filename : filenames)
    {
        FMNode* file_node = file_manager->add_file(filename.c_str());
        window_manager.open(file_node->file);
    }
}
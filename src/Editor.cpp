#include "Editor.h"
#include <string.h>
#include "log.h"
#include "FileView.h"
#include "FileSubscriber.h"

Editor::Editor() :
    bounds(Dimension(0, 0, COLS, LINES)),
    id_gen(1),
    wm(bounds)
{
    status_window = new StatusWindow(*this, Dimension(0, LINES - 2, COLS, 1));
    console_window = new ConsoleWindow(*this, Dimension(0, LINES - 1, COLS, 1));
    // file_update_handler = nullptr;
}

Editor::~Editor()
{
    // delete file_update_handler;
    delete status_window;
    delete console_window;

    for (File* file : files) {
        delete file;
    }
}

void Editor::resize()
{
    log_info("resizing screen to ln:%d col:%d", LINES, COLS);
    bounds = Dimension(0, 0, COLS, LINES);
    wm.resize(Dimension(0, 0, COLS, LINES - 2));
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
    else if (c == CTRL_C)
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

void Editor::command(const std::string& command)
{
    auto file = get_focused_file();
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
    // else if (command == "sp" || command == "split")
    // {
    //     splith();
    // }
    // else if (command == "vs" || command == "vsplit")
    // {
    //     splitv();
    // }
    // else if (command == "right")
    // {
    //     focus_right();
    // }
    // else if (command == "left")
    // {
    //     focus_left();
    // }
    // else if (command == "top")
    // {
    //     focus_top();
    // }
    // else if (command == "bottom")
    // {
    //     focus_bottom();
    // }
    // else if (command == "next")
    // {
    //     focused_node->tab_window.open_next();
    // }
    // else if (command == "prev")
    // {
    //     focused_node->tab_window.open_prev();
    // }
    // else if (command == "close")
    // {
    //     focused_node->tab_window.close_tab();
    // }
    // else if (command == "closeall")
    // {
    //     focused_node->tab_window.close_all();
    // }
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
    auto file = get_focused_file();

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
    auto file = get_focused_file();
    assert(file);
    assert(file->normal_mode_buffer.size() == 2);

    if (file->normal_mode_buffer == "gg") {
        file->goto_line(0);
    }

    file->normal_mode_buffer = "";
}

void Editor::handle_normal_mode_event(unsigned c)
{
    auto file = get_focused_file();

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
    // auto file = get_focused_file();
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
    wm.show();
    status_window->show();
    console_window->show();
}

void Editor::draw()
{
    wm.draw();
    status_window->draw();
    console_window->draw();
}

ListNode<EmptyView*>* Editor::find_tab_by_file(Tabs* tabs, File* file) {
    for (ListNode<EmptyView*>* itr = tabs.head; itr; itr = itr->next) {
    }

    return nullptr;
}

void Editor::open(const std::vector<std::string>& filenames)
{
    wm.init();
    for (const std::string& filename : filenames)
    {
        File* file = get_file(filename);
        if (!file) { file = add_file(filename); }
        Tabs* tabs = wm.focused_node.content;
        ListNode<EmptyView*>* tab_node = find_tab_by_file(tabs, file);
        if (tab_node) { tabs->open(tab_node); }
        else { tabs->open(new EmptyView(filename)); }
    }
}

File* Editor::add_file(const std::string& filename) {
    File* new_file = new File(id_gen.next(), filename);
    // new_file->add_subscriber(file_update_handler);
    new_file->load_file();
    files.push_back(new_file);
    return new_file;
}

File* Editor::get_file(const std::string& filename) {
    for (File* file : files) {
        if (file->filename == filename) {
            return file;
        }
    }

    return nullptr;
}

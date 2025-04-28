#include "Editor.h"
#include <string.h>
#include "log.h"
#include "FileView.h"
#include "FileSubscriber.h"
#include "FileFactory.h"
#include "ViewFactory.h"

void init(Editor& editor, Dimension d)
{
    log_debug("init editor");
    editor.bounds = d;

    if (!editor.status_window || editor.status_window->bounds != d) {
        delete editor.status_window;
        editor.status_window = new StatusWindow(editor, Dimension(d.x, d.y + d.height - 2, d.width, 1));
    }

    if (!editor.console_window || editor.console_window->bounds != d) {
        delete editor.console_window;
        editor.console_window = new ConsoleWindow(editor, Dimension(d.x, d.y + d.height - 1, d.width, 1));
    }

    editor.window_manager.resize(Dimension(d.x, d.y, d.width, d.height - 2));
    editor.window_manager.init();
}

Editor::Editor(Dimension d) : bounds(d), window_manager(Dimension(d.x, d.y, d.width, d.height - 2))
{
    init(*this, d);
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

void Editor::resize(Dimension d)
{
    log_info("resizing screen to ln:%d col:%d", LINES, COLS);
    init(*this, d);
    window_manager.redraw();
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
    else if (command == "sp" || command == "split")
    {
        clear(); refresh();
        window_manager.splith();
        WindowNode<Window*>* sibling = window_manager.focused_node->sibling();
        sibling->set_content(ViewFactory::new_file_view(nullptr, sibling->bounds));
        window_manager.redraw();
        // refresh();
    }
    else if (command == "vs" || command == "vsplit")
    {
        clear(); refresh();
        window_manager.splitv();
        WindowNode<Window*>* sibling = window_manager.focused_node->sibling();
        sibling->set_content(ViewFactory::new_file_view(nullptr, sibling->bounds));
        window_manager.redraw();
        // refresh();
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
    window_manager.show();
    status_window->show();
    console_window->show();
}

void Editor::draw()
{
    window_manager.draw();
    status_window->draw();
    console_window->draw();

    if (!window_manager.get_focused_node_content()) {
        auto node = window_manager.focused_node;
        int y = node->bounds.y;
        int x = node->bounds.x;
        move(y, x);
    }
}

// ListNode<EmptyView*>* Editor::find_tab_by_file(Tabs* tabs, File* file) {
//     for (ListNode<EmptyView*>* itr = tabs.head; itr; itr = itr->next) {
//     }

//     return nullptr;
// }

WindowNode<FileView*>* find_existing_file_window(WindowManager<FileView*>& window_manager, File* file) {
    std::list<WindowNode<FileView*>*> q;
    q.push_back(window_manager.root_node);
    while (!q.empty()) {
        WindowNode<FileView*>* node = q.front();
        q.pop_front();
        if (!node) { continue; }
        if (node->content->file == file) {
            return node;
        }
        for (WindowNode<FileView*>* child : node->children) {
            q.push_back(child);
        }
    }

    return nullptr;
}

void Editor::open(const std::vector<std::string>& filenames)
{
    for (const std::string& filename : filenames)
    {
        File* file = get_file(filename);
        if (!file) { file = add_file(filename); }

        WindowNode<Window*>* window_node = nullptr; //find_existing_file_window(window_manager, file);
        if (window_node) {
            // switch to existing node
            window_manager.set_focused_node(window_node);
        }
        else {
            // update content in current node
            Window* view = ViewFactory::new_file_view(file, window_manager.focused_node->bounds);
            window_manager.set_focused_node_content(view);
        }

    }
}

File* Editor::add_file(const std::string& filename) {
    File* new_file = FileFactory::new_file(filename);
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

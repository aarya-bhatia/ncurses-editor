#include "CommandMode.h"
#include "Editor.h"

void CommandMode::command(const std::string& command)
{
    auto file = editor->get_focused_file();
    log_debug("Got command: %s", command.c_str());

    if (command == "rw") {
        editor->resize(Dimension(editor->bounds.x, editor->bounds.y, editor->bounds.width - 1, editor->bounds.height));
    }
    else if (command == "rW") {
        editor->resize(Dimension(editor->bounds.x, editor->bounds.y, editor->bounds.width + 1, editor->bounds.height));
    }
    else if (command == "close") {
        editor->window_manager.close_focused_node();
    }
    else if (command == "q" || command == "quit")
    {
        log_warn("quit flag set");
        editor->quit = true;
    }
    else if (command == "Ex")
    {
        editor->change_mode(FILE_PICKER_MODE);
        return;
    }
    else if (command == "ls")
    {
        editor->change_mode(LIST_BUFFER_MODE);
        return;
    }
    else if (command.substr(0, 5) == "open ")
    {
        std::vector<std::string> filenames = splitwords(command.substr(5), " ");
        editor->open(filenames);
    }
    else if (command == "save")
    {
        if (file) {
            file->save_file();
        }
    }
    else if (command == "sp" || command == "split")
    {
        editor->window_manager.splith();
    }
    else if (command == "vs" || command == "vsplit")
    {
        editor->window_manager.splitv();
    }
    else if (command == "right")
    {
        editor->window_manager.focus_right();
    }
    else if (command == "left")
    {
        editor->window_manager.focus_left();
    }
    else if (command == "top" || command == "up")
    {
        editor->window_manager.focus_top();
    }
    else if (command == "bottom" || command == "down")
    {
        editor->window_manager.focus_bottom();
    }
    else if (command == "tabnew")
    {
        editor->window_manager.tab_new();
    }
    else if (command == "tabprev")
    {
        editor->window_manager.tab_prev();
    }
    else if (command == "tabnext")
    {
        editor->window_manager.tab_next();
    }
    else if (command == "copy")
    {
        editor->copy_buffer = file->copy_line();
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

    editor->restore_mode();
}

void CommandMode::handle_event(unsigned c)
{
    switch (c)
    {
    case CTRL_ENTER:
        command(mode_line);
        return;

    case CTRL_BACKSPACE:
    case CTRL_DEL:
        if (mode_line.empty())
        {
            editor->restore_mode();
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
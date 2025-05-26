#include "Editor.h"
#include <string.h>
#include "log.h"
#include "FileView.h"
#include "FileSubscriber.h"
#include "FileFactory.h"
#include "FileViewFactory.h"
#include "FileUpdateHandler.h"
#include "StatusWindow.h"
#include "ConsoleWindow.h"
#include "InsertMode.h"
#include "CommandMode.h"
#include "NormalMode.h"
#include "FilePickerMode.h"

Editor::Editor(Dimension d) : bounds(d), window_manager(Dimension(d.x, d.y, d.width, d.height - 2))
{
    status_window = new StatusWindow(*this, Dimension(d.x, d.y + d.height - 2, d.width, 1));
    console_window = new ConsoleWindow(*this, Dimension(d.x, d.y + d.height - 1, d.width, 1));
    file_update_handler = new FileUpdateHandler(*this);
    editor_mode = new NormalMode(this);
    window_manager.init();
}

Editor::~Editor()
{
    delete file_update_handler;
    delete status_window;
    delete console_window;

    for (File* file : files) {
        delete file;
    }
}

FileView* Editor::get_focused_file_view() {
    Window* content = window_manager.get_current_tab()->get_focused_node()->get_content();
    return FileViewFactory::get_file_view(content);
}

File* Editor::get_focused_file() {
    FileView* file_view = get_focused_file_view();
    return file_view ? file_view->get_file() : nullptr;
}

void Editor::resize(Dimension d)
{
    log_info("resizing screen to ln:%d col:%d", LINES, COLS);
    this->bounds = d;
    status_window->resize(Dimension(d.x, d.y + d.height - 2, d.width, 1));
    console_window->resize(Dimension(d.x, d.y + d.height - 1, d.width, 1));
    window_manager.resize(Dimension(d.x, d.y, d.width, d.height - 2));
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
    else if (editor_mode) {
        editor_mode->handle_event(c);
    }
}

void Editor::change_mode(Mode mode)
{
    delete prev_editor_mode;
    prev_editor_mode = editor_mode;

    if (mode == INSERT_MODE)
    {
        editor_mode = new InsertMode(this);
    }
    else if (mode == NORMAL_MODE)
    {
        editor_mode = new NormalMode(this);
    }
    else if (mode == COMMAND_MODE)
    {
        editor_mode = new CommandMode(this);
    }
    else if (mode == FILE_PICKER_MODE)
    {
        editor_mode = new FilePickerMode(this);
    }
}

void Editor::command(const std::string& command)
{
    auto file = get_focused_file();
    log_debug("Got command: %s", command.c_str());

    if (command == "rw") {
        resize(Dimension(bounds.x, bounds.y, bounds.width - 1, bounds.height));
    }
    else if (command == "rW") {
        resize(Dimension(bounds.x, bounds.y, bounds.width + 1, bounds.height));
    }
    else if (command == "close") {
        window_manager.get_current_tab()->close_focused_node();
    }
    else if (command == "q" || command == "quit")
    {
        log_warn("quit flag set");
        quit = true;
    }
    else if (command == "Ex")
    {
        change_mode(FILE_PICKER_MODE);
        return;
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
        WindowTab* current_tab = window_manager.get_current_tab();
        current_tab->splith();
    }
    else if (command == "vs" || command == "vsplit")
    {
        WindowTab* current_tab = window_manager.get_current_tab();
        current_tab->splitv();
    }
    else if (command == "right")
    {
        window_manager.get_current_tab()->focus_right();
    }
    else if (command == "left")
    {
        window_manager.get_current_tab()->focus_left();
    }
    else if (command == "top" || command == "up")
    {
        window_manager.get_current_tab()->focus_top();
    }
    else if (command == "bottom" || command == "down")
    {
        window_manager.get_current_tab()->focus_bottom();
    }
    else if (command == "tabnew")
    {
        window_manager.tab_new();
    }
    else if (command == "tabprev")
    {
        window_manager.tab_prev();
    }
    else if (command == "tabnext")
    {
        window_manager.tab_next();
    }
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

    restore_mode();
}

void Editor::draw()
{
    window_manager.draw();
    status_window->draw();
    console_window->draw();

    // this should not happen ideally
    if (window_manager.get_current_tab()->get_focused_node()->get_content() == nullptr) {
        move(0, 0);
    }
}

void Editor::open(const std::vector<std::string>& filenames)
{
    for (const std::string& filename : filenames)
    {
        File* file = get_file(filename);
        if (!file) { file = add_file(filename); }
    }

    if (!filenames.empty()) {
        File* file = get_file(filenames.back());
        open(file);
    }
}

void Editor::open(File* file)
{
    if (!file) { return; }

    Dimension d = window_manager.get_current_tab()->get_focused_bounds();
    Window* content = FileViewFactory::create_content_window(file, d);
    window_manager.get_current_tab()->get_focused_node()->set_content(content);
}

File* Editor::add_file(const std::string& filename) {
    File* new_file = FileFactory::new_file(filename);
    new_file->add_subscriber(file_update_handler);
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

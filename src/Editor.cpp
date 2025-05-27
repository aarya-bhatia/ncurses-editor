#include "Editor.h"
#include <string.h>
#include "log.h"
#include "FileView.h"
#include "FileSubscriber.h"
#include "FileFactory.h"
#include "FileViewFactory.h"
#include "StatusWindow.h"
#include "ConsoleWindow.h"
#include "InsertMode.h"
#include "CommandMode.h"
#include "NormalMode.h"
#include "FilePickerMode.h"
#include "ListBufferMode.h"

Editor::Editor(Dimension d) : bounds(d), window_manager(Dimension(d.x, d.y, d.width, d.height - 2))
{
    status_window = new StatusWindow(*this, Dimension(d.x, d.y + d.height - 2, d.width, 1));
    console_window = new ConsoleWindow(*this, Dimension(d.x, d.y + d.height - 1, d.width, 1));
    editor_mode = new NormalMode(this);
    window_manager.init();
}

Editor::~Editor()
{
    delete status_window;
    delete console_window;

    for (File* file : files) {
        delete file;
    }
}

FileView* Editor::get_focused_file_view() {
    Window* content = window_manager.get_focused_content();
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

    switch (mode) {
    case INSERT_MODE:
        editor_mode = new InsertMode(this);
        return;
    case NORMAL_MODE:
        editor_mode = new NormalMode(this);
        return;
    case COMMAND_MODE:
    {
        editor_mode = new CommandMode(this);
        return;
    }
    case FILE_PICKER_MODE:
        editor_mode = new FilePickerMode(this);
        return;
    case LIST_BUFFER_MODE:
        editor_mode = new ListBufferMode(this);
        return;
    }
}

void Editor::draw()
{
    window_manager.draw();
    status_window->draw();
    console_window->draw();

    // this should not happen ideally
    if (window_manager.get_focused_content() == nullptr) {
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

    Window* content = FileViewFactory::create_content_window(window_manager.get_focused_bounds(), file);
    window_manager.set_focused_content(content);
}

File* Editor::add_file(const std::string& filename) {
    File* new_file = FileFactory::new_file(filename);
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

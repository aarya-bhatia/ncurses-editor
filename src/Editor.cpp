#include "Editor.h"
#include <string.h>
#include "log.h"
#include "dirent.h"
#include "FileView.h"
#include "FileSubscriber.h"
#include "FileFactory.h"
#include "FileViewFactory.h"
#include "StatusWindow.h"
#include "ConsoleWindow.h"

Editor::Editor(Dimension d) : bounds(d), window_manager(Dimension(d.x, d.y, d.width, d.height - 2))
{
    status_window = new StatusWindow(*this, Dimension(d.x, d.y + d.height - 2, d.width, 1));
    console_window = new ConsoleWindow(*this, Dimension(d.x, d.y + d.height - 1, d.width, 1));
    editor_mode = Mode::Normal;
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

    if (c == CTRL_C)
    {
        quit = true;
        return;
    }

    File* file = get_focused_file();
    if (file && file->event_handler && file->event_handler(c)) {
        return;
    }

    switch (editor_mode) {
    case Mode::Normal: handle_normal_mode_event(c); return;
    case Mode::Insert: handle_insert_mode_event(c); return;
    case Mode::Command: handle_command_mode_event(c); return;
    }
}

void Editor::change_mode(Mode mode)
{
    editor_mode = mode;
    mode_line = "";
    log_info("Mode changed to %s", mode_name().c_str());
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
    else {
        int y, x;
        FileViewFactory::get_file_view(window_manager.get_focused_content())->get_absolute_cursor(y, x);
        move(y, x);
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

std::string Editor::mode_name() const {
    switch (editor_mode) {
    case Mode::Normal: return "NORMAL";
    case Mode::Insert: return "INSERT";
    case Mode::Command: return "COMMAND";
    }
    return "UNKNOWN";
}

void Editor::handle_insert_mode_event(unsigned c) {
    auto file = get_focused_file();

    switch (c)
    {
    case CTRL_ESCAPE:
        if (file) {
            file->cursor_left();
        }
        change_mode(Mode::Normal);
        break;

    default:
        if (file->readonly) {
            break;
        }

        if (PRINTABLE(c))
        {
            file->insert_character(c);
        }
    }
}

void Editor::handle_normal_mode_event(unsigned c) {
    auto file = get_focused_file();

    if (!file) {
        switch (c) {
        case ':':
            change_mode(Mode::Command);
            return;
        }

        return;
    }

    if (file->normal_mode_buffer.size() > 0) {
        file->normal_mode_buffer += c;

        if (file->normal_mode_buffer == "gg") {
            file->goto_line(0);
        }
        else if (file->normal_mode_buffer == "yy") {
            copy_buffer = file->copy_line();
        }
        else if (file->normal_mode_buffer == "dd") {
            file->remove_line();
        }
        else if (file->normal_mode_buffer == " ws") {
            window_manager.splith();
        }
        else if (file->normal_mode_buffer == " wc") {
            window_manager.close_focused_node();
        }
        else if (file->normal_mode_buffer == " wv") {
            window_manager.splitv();
        }
        else if (file->normal_mode_buffer == " wl") {
            window_manager.focus_right();
        }
        else if (file->normal_mode_buffer == " wh") {
            window_manager.focus_left();
        }
        else if (file->normal_mode_buffer == " wj") {
            window_manager.focus_bottom();
        }
        else if (file->normal_mode_buffer == " wk") {
            window_manager.focus_top();
        }
        else if (file->normal_mode_buffer == " tn") {
            window_manager.tab_next();
        }
        else if (file->normal_mode_buffer == " te") {
            window_manager.tab_new();
        }
        else if (file->normal_mode_buffer == " tp") {
            window_manager.tab_prev();
        }
        else if (file->normal_mode_buffer == " tc") {
            window_manager.tab_close();
        }
        else if (file->normal_mode_buffer == " w") {
            return;
        }
        else if (file->normal_mode_buffer == " t") {
            return;
        }

        file->normal_mode_buffer = "";
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

    case 'x':
        file->remove_character();
        return;

    case 'o':
        file->insert_line_below();
        return;

    case 'i':
        change_mode(Mode::Insert);
        return;

    case 'y':
        file->normal_mode_buffer += c;
        return;

    case 'p':
        if (!copy_buffer.empty()) file->paste_below(copy_buffer);
        return;

    case 'P':
        if (!copy_buffer.empty()) file->paste_above(copy_buffer);
        return;

    case CTRL_ESCAPE:
        file->normal_mode_buffer.clear();
        return;

    case ':':
        change_mode(Mode::Command);
        return;

    default:
        file->normal_mode_buffer += c;
        return;
    }
}

void Editor::handle_command_mode_event(unsigned c) {
    switch (c)
    {
    case CTRL_ENTER:
        command(mode_line);
        change_mode(Mode::Normal);
        return;

    case CTRL_BACKSPACE:
    case CTRL_DEL:
        if (mode_line.empty())
        {
            change_mode(Mode::Normal);
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

void Editor::new_file_picker_window() {
    File* _file = new File();
    DIR* dirp = opendir(".");
    if (!dirp) {
        perror("opendir");
        return;
    }

    _file->lines.clear();

    struct dirent* e;
    while ((e = readdir(dirp)) != nullptr)
    {
        if (e->d_type == DT_REG)
        {
            std::string name(e->d_name);
            log_debug("listing: %s", name.c_str());
            std::list<char> chars(name.begin(), name.end());
            _file->lines.push_back(chars);
        }
    }

    closedir(dirp);

    if (_file->lines.empty())
    {
        _file->lines.push_back({});
    }

    _file->cursor.x = 0;
    _file->cursor.y = 0;
    _file->cursor.line = _file->lines.begin();
    _file->cursor.col = _file->cursor.line->begin();

    _file->event_handler = [this](unsigned c) -> bool {
        File* file = this->get_focused_file();
        std::string filename = file->get_current_line();
        switch (c)
        {
        case CTRL_ENTER:
            log_info("opening file: %s", filename.c_str());
            if (!filename.empty()) {
                this->open({ filename });
                delete file;
                file = nullptr;
            }
            return true;
        default: return false;
        }
        };

    open(_file);
}

void Editor::new_list_buffers_window() {
    File* _file = FileFactory::new_file();
    _file->readonly = true;
    _file->lines.clear();

    for (File* file : files) {
        std::string s = file->filename;
        _file->lines.emplace_back(s.begin(), s.end());
    }

    if (_file->lines.empty())
    {
        _file->lines.push_back({});
    }

    _file->cursor.x = 0;
    _file->cursor.y = 0;
    _file->cursor.line = _file->lines.begin();
    _file->cursor.col = _file->cursor.line->begin();

    _file->event_handler = [this](unsigned c) -> bool {
        File* file = this->get_focused_file();
        std::string filename = file->get_current_line();
        switch (c)
        {
        case CTRL_ENTER:
            log_info("opening buffer: %s", filename.c_str());
            if (!filename.empty()) {
                this->open({ filename });
                delete file;
                file = nullptr;
            }
            return true;
        default: return false;
        }
        };

    open(_file);

}

void Editor::command(const std::string& command) {
    auto file = get_focused_file();
    log_debug("Got command: %s", command.c_str());

    if (command == "rw") {
        resize(Dimension(bounds.x, bounds.y, bounds.width - 1, bounds.height));
    }
    else if (command == "rW") {
        resize(Dimension(bounds.x, bounds.y, bounds.width + 1, bounds.height));
    }
    else if (command == "close") {
        window_manager.close_focused_node();
    }
    else if (command == "q" || command == "quit")
    {
        log_warn("quit flag set");
        quit = true;
    }
    else if (command == "Ex")
    {
        new_file_picker_window();
    }
    else if (command == "ls")
    {
        new_list_buffers_window();
    }
    else if (command.substr(0, 5) == "open " || command.substr(0, 5) == "edit ")
    {
        std::vector<std::string> filenames = splitwords(command.substr(5), " ");
        open(filenames);
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
    else if (command == "top" || command == "up")
    {
        window_manager.focus_top();
    }
    else if (command == "bottom" || command == "down")
    {
        window_manager.focus_bottom();
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
    else if (command == "tabclose")
    {
        window_manager.tab_close();
    }
    else if (command == "copy")
    {
        copy_buffer = file->copy_line();
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
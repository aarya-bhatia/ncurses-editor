#include "FilePickerMode.h"
#include "dirent.h"
#include "Editor.h"
#include "FileViewFactory.h"

FilePickerMode::FilePickerMode(Editor* editor) : _normal(editor)
{
    this->editor = editor;

    _file = new File();

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

    _file->add_subscriber((FileSubscriber*)editor->file_update_handler);

    Window* new_content = FileViewFactory::create_content_window(_file);
    editor->window_manager.get_current_tab()->get_focused_node()->set_content(new_content);
}

FilePickerMode::~FilePickerMode()
{
    delete _file;
}

void FilePickerMode::handle_event(unsigned c)
{
    std::string filename = _file->get_current_line();

    switch (c)
    {
    case CTRL_ENTER:
        log_info("opening file: %s", filename.c_str());
        if (!filename.empty()) {
            editor->open({ filename });
            editor->change_mode(NORMAL_MODE);
        }
        return;
    default:
        _normal.handle_event(c);
        return;
    }
}
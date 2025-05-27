#pragma once

#include <list>
#include <string>
#include <map>
#include <vector>
#include <assert.h>
#include "common.h"
#include "FileView.h"
#include "WindowNode.h"
#include "WindowManager.h"
#include "WindowNode.h"
#include "EditorMode.h"

struct StatusWindow;
struct ConsoleWindow;

class Editor
{
private:
    bool quit = false;

    EditorMode* prev_editor_mode = nullptr;
    EditorMode* editor_mode = nullptr;
    Dimension bounds;
    WindowManager window_manager;
    StatusWindow* status_window = nullptr;
    ConsoleWindow* console_window = nullptr;
    std::list<File*> files;

    std::string copy_buffer = "";

public:
    Editor(Dimension d);
    ~Editor();

    bool ok() const { return quit; }

    void handle_event(unsigned c);
    void draw();
    void show();
    void resize(Dimension d);
    void open(const std::vector<std::string>& filenames);
    void open(File*);

    void change_mode(Mode mode);

    void restore_mode() {
        editor_mode = prev_editor_mode;
        prev_editor_mode = nullptr;
    }

private:
    FileView* get_focused_file_view();
    File* get_focused_file();

    File* add_file(const std::string& filename);
    File* get_file(const std::string& filename);

    void handle_command_mode_event(unsigned c);
    void handle_insert_mode_event(unsigned c);
    void handle_normal_mode_event(unsigned c);
    void handle_normal_mode_two_key_seq();

    void _init(Dimension);

    friend struct StatusWindow;
    friend struct ConsoleWindow;
    friend struct FileUpdateHandler;
    friend struct InsertMode;
    friend struct CommandMode;
    friend struct NormalMode;
    friend struct FilePickerMode;
    friend struct ListBufferMode;
};

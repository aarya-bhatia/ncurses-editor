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

    Dimension bounds;
    WindowManager window_manager;
    StatusWindow* status_window = nullptr;
    ConsoleWindow* console_window = nullptr;
    std::list<File*> files;

    std::string mode_line = "";
    enum class Mode
    {
        Normal,
        Insert,
        Command,
    } editor_mode = Mode::Normal;

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

    std::string mode_name() const;

    void change_mode(Mode mode);
    void command(const std::string& command);

    std::string get_console_string() const { return editor_mode == Mode::Command ? ":" + mode_line : ""; }

private:
    void handle_insert_mode_event(unsigned c);
    void handle_normal_mode_event(unsigned c);
    void handle_command_mode_event(unsigned c);

    void new_file_picker_window();
    void new_list_buffers_window();

    FileView* get_focused_file_view();
    File* get_focused_file();

    File* add_file(const std::string& filename);
    File* get_file(const std::string& filename);

    void _init(Dimension);

    friend struct StatusWindow;
    friend struct ConsoleWindow;
};

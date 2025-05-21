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

struct FileUpdateHandler;
struct StatusWindow;
struct ConsoleWindow;

class Editor
{
private:
    Mode mode = NORMAL_MODE;
    bool quit = false;

    Cursor prev_cursor;

    Dimension bounds;
    WindowManager window_manager;
    StatusWindow* status_window = nullptr;
    ConsoleWindow* console_window = nullptr;
    std::string mode_line = "";
    std::string statusline = "";
    FileUpdateHandler* file_update_handler;
    std::list<File*> files;

public:
    Editor(Dimension d);
    ~Editor();

    bool ok() const { return quit; }

    void command(const std::string& command);
    void handle_event(unsigned c);
    void draw();
    void show();
    void resize(Dimension d);
    void open(const std::vector<std::string>& filenames);

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
};

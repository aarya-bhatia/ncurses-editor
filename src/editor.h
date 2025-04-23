#pragma once

#include <list>
#include <string>
#include <map>
#include <vector>
#include <assert.h>
#include "common.h"
#include "FileView.h"
#include "WindowManager.h"
#include "StatusWindow.h"
#include "ConsoleWindow.h"
#include "Command.h"
#include "FileUpdateHandler.h"
#include "SequenceGenerator.h"

enum Mode
{
    NORMAL_MODE,
    INSERT_MODE,
    COMMAND_MODE
};

static std::map<Mode, const char*> mode_names = {
    {NORMAL_MODE, "NORMAL"},
    {COMMAND_MODE, "COMMAND"},
    {INSERT_MODE, "INSERT"},
};

struct Editor
{
    LinearSequenceGenerator<int> id_gen;
    WindowManager* window_manager;
    FileUpdateHandler* file_update_handler;
    StatusWindow* status_window;
    ConsoleWindow* console_window;

    std::list<File*> files;

    std::string mode_line = "";
    std::string statusline = "";

    Mode mode = NORMAL_MODE;
    bool quit = false;

    Editor();
    ~Editor();

    Window* get_current_view();
    File* get_current_file();

    void command(const std::string& command);

    void handle_event(unsigned c);
    void handle_command_mode_event(unsigned c);
    void handle_insert_mode_event(unsigned c);
    void handle_normal_mode_event(unsigned c);
    void handle_normal_mode_two_key_seq();

    void draw();
    void show();
    void resize();

    void open(const std::vector<std::string>& filenames);
    File* add_file(const std::string& filename);
    File* get_file(const std::string& filename);
};

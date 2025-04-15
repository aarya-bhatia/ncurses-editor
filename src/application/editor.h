#pragma once

#include <list>
#include <string>
#include <map>
#include <vector>
#include <assert.h>
#include "common.h"
#include "FileManager.h"
#include "window/IWindowManager.h"
#include "StatusWindow.h"
#include "ConsoleWindow.h"
#include "commands/Command.h"

#include <ncurses.h>

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
    std::unique_ptr<FileManager> file_manager;
    std::shared_ptr<IWindowManager> window_manager;
    std::unique_ptr<StatusWindow> status_window;
    std::unique_ptr<ConsoleWindow> console_window;

    std::vector<std::unique_ptr<Command>> commands;

    std::string mode_line = "";
    std::string statusline = "";

    Mode mode = NORMAL_MODE;
    bool quit = false;

    Editor();

    FileView* get_current_view();
    std::shared_ptr<File> get_current_file();

    void command(const std::string& command);

    void handle_event(unsigned c);
    void handle_command_mode_event(unsigned c);
    void handle_insert_mode_event(unsigned c);
    void handle_normal_mode_event(unsigned c);
    void handle_normal_mode_two_key_seq();

    void draw();
    void show();

    void force_redraw_editor();
    void redraw_line(Cursor);

    void resize();

    void open(const std::vector<std::string>& filenames);
};

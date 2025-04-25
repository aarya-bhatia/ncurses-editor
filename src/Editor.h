#pragma once

#include <list>
#include <string>
#include <map>
#include <vector>
#include <assert.h>
#include "common.h"
#include "FileView.h"
#include "WindowNode.h"
#include "StatusWindow.h"
#include "ConsoleWindow.h"
#include "FileUpdateHandler.h"
#include "WindowManager.h"

struct Editor
{
    Mode mode = NORMAL_MODE;
    bool quit = false;

    Dimension bounds;
    WindowManager<FileView*> window_manager;
    StatusWindow* status_window = nullptr;
    ConsoleWindow* console_window = nullptr;
    std::list<File*> files;
    std::string mode_line = "";
    std::string statusline = "";

    // FileUpdateHandler* file_update_handler;
    // std::unordered_map<File*, std::vector<WindowNode*>> map_file_to_window_node;
    // std::unordered_map<File*, std::vector<Window*>> map_file_to_window;

    Editor(Dimension d);
    ~Editor();

    File* get_focused_file() {
        FileView* view = window_manager.get_focused_node_content();
        return view ? view->file : nullptr;
    }

    Window* get_focused_window() {
        return window_manager.get_focused_node_content();
    }

    void command(const std::string& command);

    void handle_event(unsigned c);
    void handle_command_mode_event(unsigned c);
    void handle_insert_mode_event(unsigned c);
    void handle_normal_mode_event(unsigned c);
    void handle_normal_mode_two_key_seq();

    void draw();
    void show();
    void resize(Dimension d);

    void open(const std::vector<std::string>& filenames);

    File* add_file(const std::string& filename);
    File* get_file(const std::string& filename);
};

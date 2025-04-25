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
#include "Command.h"
#include "FileUpdateHandler.h"
#include "SequenceGenerator.h"
#include "WindowManager.h"
#include "TabWindow.h"
#include "EmptyView.h"

struct Editor
{
    Dimension bounds;
    LinearSequenceGenerator<int> id_gen;

    using Tabs = TabWindow<EmptyView>;
    WindowManager<Tabs> wm;

    // FileUpdateHandler* file_update_handler;
    StatusWindow* status_window;
    ConsoleWindow* console_window;

    std::list<File*> files;

    std::string mode_line = "";
    std::string statusline = "";

    // std::unordered_map<File*, std::vector<WindowNode*>> map_file_to_window_node;
    // std::unordered_map<File*, std::vector<Window*>> map_file_to_window;

    Mode mode = NORMAL_MODE;
    bool quit = false;

    Editor();
    ~Editor();

    File* get_focused_file() {
        return nullptr;
    }

    void open(const std::vector<std::string>& filenames);
    void command(const std::string& command);
    void handle_event(unsigned c);
    void handle_command_mode_event(unsigned c);
    void handle_insert_mode_event(unsigned c);
    void handle_normal_mode_event(unsigned c);
    void handle_normal_mode_two_key_seq();

    void draw();
    void show();
    void resize();

    File* add_file(const std::string& filename);
    File* get_file(const std::string& filename);

    ListNode<Tabs*>* find_tab_by_file(Tabs& tabs, File* file);
};

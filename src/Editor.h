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
#include "WindowManager.h"
#include "WindowNode.h"

struct FileUpdateHandler;

struct Editor
{
    Mode mode = NORMAL_MODE;
    bool quit = false;

    Dimension bounds;
    WindowManager window_manager;
    StatusWindow* status_window = nullptr;
    ConsoleWindow* console_window = nullptr;
    std::string mode_line = "";
    std::string statusline = "";

    FileUpdateHandler* file_update_handler;

    std::list<File*> files;

    Editor(Dimension d);
    ~Editor();

    FileView* get_focused_file_view() {
        WindowTab* tab = window_manager.get_current_tab();
        assert(tab);
        Window* view = tab->get_focused_node_content();
        FileView* file_view = view->get_file_view();
        return file_view;
    }

    File* get_focused_file() {
        FileView* file_view = get_focused_file_view();
        return file_view ? file_view->get_file() : nullptr;
    }

    void command(const std::string& command);

    void handle_event(unsigned c);
    void handle_command_mode_event(unsigned c);
    void handle_insert_mode_event(unsigned c);
    void handle_normal_mode_event(unsigned c);
    void handle_normal_mode_two_key_seq();

    void draw();
    void draw_cursor();
    void show();
    void resize(Dimension d);

    void open(const std::vector<std::string>& filenames);
    FileView* open_file_view(File*);

    File* add_file(const std::string& filename);
    File* get_file(const std::string& filename);
};

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

struct Editor
{
    LinearSequenceGenerator<int> id_gen;
    // FileUpdateHandler* file_update_handler;
    StatusWindow* status_window;
    ConsoleWindow* console_window;

    std::list<File*> files;

    std::string mode_line = "";
    std::string statusline = "";

    WindowNode* root_node = nullptr;
    Dimension bounds;

    WindowNode* focused_node = nullptr;
    WindowNode* prev_focused_node = nullptr;

    std::unordered_map<File*, std::vector<WindowNode*>> map_file_to_window_node;
    std::unordered_map<File*, std::vector<Window*>> map_file_to_window;

    Mode mode = NORMAL_MODE;
    bool quit = false;

    Editor();
    ~Editor();

    WindowNode* get_focused_node() { return focused_node; }
    WindowNode* get_prev_focused_node() { return prev_focused_node; }

    bool is_focused_node(WindowNode* node) const { return node == focused_node; }
    bool was_focused_node(WindowNode* node) const { return node == prev_focused_node; }
    bool focused_node_changed() const { return prev_focused_node != focused_node; }

    // Window* get_focused_window() { return focused_node ? focused_node->get_focused_window() : nullptr; }
    // Window* get_prev_focused_window() { return prev_focused_node ? prev_focused_node->get_focused_window() : nullptr; }

    // bool is_focused_window(Window* window) const { return get_focused_window() == window; }
    // bool was_focused_window(Window* window) const { return get_prev_focused_window() == window; }
    // bool focused_window_changed() const { return get_focused_window() != get_prev_focused_window(); }

    // File* get_focused_file() { return focused_node && focused_node->get_focused_window() ? focused_node->get_focused_window()->get_file() : nullptr; }
    // File* get_prev_focused_file() { return prev_focused_node && prev_focused_node->get_focused_window() ? prev_focused_node->get_focused_window()->get_file() : nullptr; }

    // bool is_focused_file(File* file) const { return get_focused_file() == file; }
    // bool was_focused_file(File* file) const { return get_prev_focused_file() == file; }
    // bool focused_file_changed() const { return get_focused_file() != get_prev_focused_file(); }

    void set_focused_node(WindowNode* node) {
        prev_focused_node = focused_node;
        focused_node = node;
    }

    void refocus() {
        if (prev_focused_node) { prev_focused_node->unfocus(); }
        if (focused_node) { focused_node->focus(); }
    }

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

    void open_tab(File* file) {
        assert(focused_node);
        Window* view = focused_node->open_tab(file);
        assert(focused_node->get_window() == view);
        assert(focused_node->layout == WindowNode::Layout::NORMAL);
    }

    bool splith() {
        assert(focused_node);
        if (!focused_node->splith_allowed())
        {
            log_debug("split failed");
            return false;
        }
        focused_node->splith();
        assert(focused_node->layout == WindowNode::Layout::HSPLIT);
        set_current_node(focused_node->get_top_child());
        return true;
    }

    bool splitv() {
        assert(focused_node);
        if (!focused_node->splitv_allowed()) {
            log_debug("split failed");
            return false;
        }
        focused_node->splitv();
        assert(focused_node->layout == WindowNode::Layout::VSPLIT);
        set_current_node(focused_node->get_left_child());
        return true;
    }

    void set_current_node(WindowNode* node) {
        assert(node);
        if (focused_node) {
            focused_node->unfocus();
        }
        focused_node = node;
        focused_node->focus();
    }

    void focus_right() {
        assert(focused_node);
        WindowNode* new_node = focused_node->find_right_adjacent_node();
        if (new_node) {
            if (new_node->layout != WindowNode::Layout::NORMAL) { log_warn("illegal focus node");return; }
            set_current_node(new_node);
        }
        else { log_warn("did not find any focusable nodes"); }
    }

    void focus_left() {
        assert(focused_node);
        WindowNode* new_node = focused_node->find_left_adjacent_node();
        if (new_node) {
            if (new_node->layout != WindowNode::Layout::NORMAL) { log_warn("illegal focus node");return; }
            set_current_node(new_node);
        }
        else { log_warn("did not find any focusable nodes"); }
    }

    void focus_top() {
        assert(focused_node);
        WindowNode* new_node = focused_node->find_top_adjacent_node();
        if (new_node) {
            if (new_node->layout != WindowNode::Layout::NORMAL) { log_warn("illegal focus node");return; }
            set_current_node(new_node);
        }
        else { log_warn("did not find any focusable nodes"); }
    }

    void focus_bottom() {
        assert(focused_node);
        WindowNode* new_node = focused_node->find_bottom_adjacent_node();
        if (new_node) {
            if (new_node->layout != WindowNode::Layout::NORMAL) { log_warn("illegal focus node");return; }
            set_current_node(new_node);
        }
        else { log_warn("did not find any focusable nodes"); }
    }
};

#pragma once

#include "WindowNode.h"
#include "Window.h"

struct WindowManager {
    WindowNode* root_node = nullptr;
    WindowNode* focused_node = nullptr;
    Dimension bounds;

    WindowManager(Dimension d) : bounds(d) { init(); }
    ~WindowManager() { destroy(); }

    void set_focused_node_content(Window* content) {
        if (focused_node->content == content) { return; }
        if (focused_node->content) { focused_node->content->unfocus(); }
        focused_node->set_content(content);
        focused_node->content->focus();
    }

    void set_focused_node(WindowNode* node) {
        focused_node->unfocus();
        focused_node = node;
        focused_node->focus();
    }

    Window* get_focused_node_content() {
        return focused_node->content;
    }

    void redraw() {
        root_node->redraw();
    }

    void draw() {
        root_node->draw();
    }

    void show() {
        root_node->show();
    }

    void resize(Dimension d) {
        if (bounds == d) { return; }
        bounds = d;
        root_node->resize(d);
    }

    void destroy() {
        delete root_node;
        focused_node = root_node = nullptr;
    }

    void init() {
        if (focused_node) { return; }
        log_debug("init window manager");
        root_node = new WindowNode(bounds, nullptr);
        root_node->bounds = bounds;
        focused_node = root_node;
    }

    bool splith() {
        if (!focused_node->splith_allowed()) { return false; }
        focused_node->splith();
        assert(focused_node->layout == WindowNode::Layout::HSPLIT);
        set_focused_node(focused_node->get_top_child());
        return true;
    }

    bool splitv() {
        if (!focused_node->splitv_allowed()) { return false; }
        focused_node->splitv();
        assert(focused_node->layout == WindowNode::Layout::VSPLIT);
        set_focused_node(focused_node->get_left_child());
        return true;
    }

    bool focus_right() {
        WindowNode* new_node = focused_node->find_right_adjacent_node();
        if (!new_node) {
            return false;
        }
        assert(new_node->layout == WindowNode::Layout::NORMAL);
        set_focused_node(new_node);
        return true;
    }

    bool focus_left() {
        WindowNode* new_node = focused_node->find_left_adjacent_node();
        if (!new_node) {
            return false;
        }
        assert(new_node->layout == WindowNode::Layout::NORMAL);
        set_focused_node(new_node);
        return true;
    }

    bool focus_top() {
        WindowNode* new_node = focused_node->find_top_adjacent_node();
        if (!new_node) {
            return false;
        }
        assert(new_node->layout == WindowNode::Layout::NORMAL);
        set_focused_node(new_node);
        return true;
    }

    bool focus_bottom() {
        WindowNode* new_node = focused_node->find_bottom_adjacent_node();
        if (!new_node) {
            return false;
        }
        assert(new_node->layout == WindowNode::Layout::NORMAL);
        set_focused_node(new_node);
        return true;
    }
};
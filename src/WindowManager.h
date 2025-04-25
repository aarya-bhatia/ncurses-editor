#pragma once

#include "WindowNode.h"

template<typename T>
struct WindowManager {
    WindowNode<T>* root_node = nullptr;
    WindowNode<T>* focused_node = nullptr;
    Dimension bounds;

    WindowManager(Dimension d) : bounds(d) { init(); }
    ~WindowManager() { destroy(); }

    void set_focused_content(T& content) {
        focused_node->content = content;
    }

    T& get_focused_content() {
        return focused_node->content;
    }

    void draw() {
        focused_node->draw();
    }

    void show() {
        focused_node->show();
    }

    void resize(Dimension d) {
        bounds = d;
        root_node->resize(d);
    }

    void destroy() {
        delete root_node;
        focused_node = root_node = nullptr;
    }

    void init() {
        if (focused_node) { return; }

        root_node = new WindowNode<T>(bounds, nullptr);
        root_node->bounds = bounds;
        focused_node = root_node;
    }

    bool splith() {
        if (!focused_node->splith_allowed()) { return false; }
        focused_node->splith();
        assert(focused_node->layout == WindowNode<T>::Layout::HSPLIT);
        set_current_node(focused_node->get_top_child());
        return true;
    }

    bool splitv() {
        if (!focused_node->splitv_allowed()) { return false; }
        focused_node->splitv();
        assert(focused_node->layout == WindowNode<T>::Layout::VSPLIT);
        set_current_node(focused_node->get_left_child());
        return true;
    }

    void set_current_node(WindowNode<T>* node) {
        focused_node->unfocus();
        focused_node = node;
        focused_node->focus();
    }

    bool focus_right() {
        WindowNode<T>* new_node = focused_node->find_right_adjacent_node();
        if (!new_node) {
            return false;
        }
        assert(new_node->layout == WindowNode<T>::Layout::NORMAL);
        set_current_node(new_node);
        return true;
    }

    bool focus_left() {
        WindowNode<T>* new_node = focused_node->find_left_adjacent_node();
        if (!new_node) {
            return false;
        }
        assert(new_node->layout == WindowNode<T>::Layout::NORMAL);
        set_current_node(new_node);
        return true;
    }

    bool focus_top() {
        WindowNode<T>* new_node = focused_node->find_top_adjacent_node();
        if (!new_node) {
            return false;
        }
        assert(new_node->layout == WindowNode<T>::Layout::NORMAL);
        set_current_node(new_node);
        return true;
    }

    bool focus_bottom() {
        WindowNode<T>* new_node = focused_node->find_bottom_adjacent_node();
        if (!new_node) {
            return false;
        }
        assert(new_node->layout == WindowNode<T>::Layout::NORMAL);
        set_current_node(new_node);
        return true;
    }
};
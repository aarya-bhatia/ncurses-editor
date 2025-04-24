#pragma once

#include "Types.h"
#include <vector>
#include <list>
#include "Dimension.h"
#include "Window.h"
#include "log.h"
#include "File.h"
#include "FileView.h"
#include "ViewFactory.h"
#include "TabWindow.h"

struct WindowNode : public IDrawable, IFocusable
{
    std::vector<WindowNode*> children;
    Dimension bounds;
    WindowNode* parent = nullptr;
    bool focused = false;
    enum Layout { NORMAL, HSPLIT, VSPLIT }layout = NORMAL;

    TabWindow tab_window;

    WindowNode(Dimension bounds, WindowNode* parent) : bounds(bounds), parent(parent), tab_window(bounds) {}

    ~WindowNode()
    {
        for (WindowNode* child : children) {
            delete child;
        }
    }

    TabWindow& get_tab_window() { return tab_window; }

    Window* get_window() {
        return tab_window.get_focused_window();
    }

    Window* open_tab(File* f)
    {
        ListNode<Window*>* tab = tab_window.find_tab_by_file(f);
        if (tab) {
            return tab->data;
        }
        Window* file_window = ViewFactory::new_file_view(f, bounds);
        tab_window.open(file_window);
        return file_window;
    }

    void focus() override {
        log_debug("focus node %s", bounds.debug_string().c_str());
        focused = true;
        tab_window.focus();
    }

    void unfocus() override {
        log_debug("unfocus node %s", bounds.debug_string().c_str());
        focused = false;
        tab_window.unfocus();
    }

    bool splitv_allowed() { return bounds.width / 2 >= 3; }
    bool splith_allowed() { return bounds.height / 2 >= 3; }

    void resize(Dimension d) override;

    void close_tab();

    // TODO: transfer this nodes tab_window to a child?
    void splith();
    void splitv();

    void draw() override
    {
        if (children.empty()) {
            tab_window.draw();
            return;
        }

        for (auto* child : children) { child->draw(); }
    }

    void show() override
    {
        if (children.empty()) { tab_window.show(); return; }
        for (auto* child : children) { child->show(); }
    }

    int count_nodes()
    {
        int ans = 1;
        for (auto* child : children) { ans += child->count_nodes(); }
        return ans;
    }

    WindowNode* get_left_child()
    {
        if (layout == VSPLIT) return children[0];
        return nullptr;
    }

    WindowNode* get_right_child()
    {
        if (layout == VSPLIT) return children[1];
        return nullptr;
    }

    WindowNode* get_top_child()
    {
        if (layout == HSPLIT) return children[0];
        return nullptr;
    }

    WindowNode* get_bottom_child()
    {
        if (layout == HSPLIT) return children[1];
        return nullptr;
    }

    WindowNode* find_left_content_node()
    {
        if (layout == NORMAL) {
            return this;
        }

        if (layout != VSPLIT) {
            return children[0]->find_left_content_node();
        }

        return children[0]->find_left_content_node();
    }

    WindowNode* find_right_content_node()
    {
        if (layout == NORMAL) {
            return this;
        }

        if (layout != VSPLIT) {
            return children[0]->find_right_content_node();
        }

        return children[1]->find_right_content_node();
    }

    WindowNode* find_top_content_node()
    {
        if (layout == NORMAL) {
            return this;
        }

        if (layout != HSPLIT) {
            return children[0]->find_top_content_node();
        }

        return children[0]->find_top_content_node();
    }

    WindowNode* find_bottom_content_node()
    {
        if (layout == NORMAL) {
            return this;
        }

        if (layout != HSPLIT) {
            return children[0]->find_bottom_content_node();
        }

        return children[1]->find_bottom_content_node();
    }

    WindowNode* find_right_adjacent_node()
    {
        if (!parent) {
            return nullptr;
        }

        if (this == parent->get_left_child()) {
            return parent->get_right_child()->find_left_content_node();
        }

        return parent->find_right_adjacent_node();
    }

    WindowNode* find_left_adjacent_node()
    {
        if (!parent) {
            return nullptr;
        }

        if (this == parent->get_right_child()) {
            return parent->get_left_child()->find_right_content_node();
        }

        return parent->find_left_adjacent_node();
    }

    WindowNode* find_top_adjacent_node()
    {
        if (!parent) {
            return nullptr;
        }

        if (this == parent->get_bottom_child()) {
            return parent->get_top_child()->find_bottom_content_node();
        }

        return parent->find_top_adjacent_node();
    }

    WindowNode* find_bottom_adjacent_node()
    {
        if (!parent) {
            return nullptr;
        }

        if (this == parent->get_top_child()) {
            return parent->get_bottom_child()->find_top_content_node();
        }

        return parent->find_bottom_adjacent_node();
    }
};
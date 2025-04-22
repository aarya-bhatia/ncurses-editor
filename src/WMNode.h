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

struct WMNode : public IDrawable, IFocusable
{
    std::vector<WMNode*> children;
    Dimension bounds;
    WMNode* parent = nullptr;
    bool focused = false;
    enum Layout { NORMAL, HSPLIT, VSPLIT }layout = NORMAL;

    TabWindow tabs;

    WMNode(Dimension bounds, WMNode* parent) : bounds(bounds), parent(parent), tabs(bounds) {}

    ~WMNode()
    {
        for (WMNode* child : children) {
            delete child;
        }
    }

    Window* get_window() {
        return tabs.current_window();
    }

    Window* open_tab(File* f)
    {
        Window* tab_window = ViewFactory::new_file_view(f, bounds);
        tabs.add_tab(tab_window);
        tabs.open_next();
        return tabs.current_window();
    }

    void focus() override {
        log_debug("focus node %s", bounds.debug_string().c_str());
        focused = true;
        tabs.focus();
    }

    void unfocus() override {
        log_debug("unfocus node %s", bounds.debug_string().c_str());
        focused = false;
        tabs.unfocus();
    }

    bool splitv_allowed() { return bounds.width / 2 >= 3; }
    bool splith_allowed() { return bounds.height / 2 >= 3; }

    void resize(Dimension d) override;

    void close_tab();

    // TODO: transfer this nodes tabs to a child?
    void splith();
    void splitv();

    void draw() override
    {
        for (auto* child : children) { child->draw(); }

        tabs.draw();
    }

    void show() override
    {
        for (auto* child : children) { child->show(); }

        tabs.show();
    }

    int count_nodes()
    {
        int ans = 1;
        for (auto* child : children) { ans += child->count_nodes(); }
        return ans;
    }

    WMNode* get_left_child()
    {
        if (layout == VSPLIT) return children[0];
        return nullptr;
    }

    WMNode* get_right_child()
    {
        if (layout == VSPLIT) return children[1];
        return nullptr;
    }

    WMNode* get_top_child()
    {
        if (layout == HSPLIT) return children[0];
        return nullptr;
    }

    WMNode* get_bottom_child()
    {
        if (layout == HSPLIT) return children[1];
        return nullptr;
    }

    WMNode* find_left_content_node()
    {
        if (layout == NORMAL) {
            return this;
        }

        if (layout != VSPLIT) {
            return children[0]->find_left_content_node();
        }

        return children[0]->find_left_content_node();
    }

    WMNode* find_right_content_node()
    {
        if (layout == NORMAL) {
            return this;
        }

        if (layout != VSPLIT) {
            return children[0]->find_right_content_node();
        }

        return children[1]->find_right_content_node();
    }

    WMNode* find_top_content_node()
    {
        if (layout == NORMAL) {
            return this;
        }

        if (layout != HSPLIT) {
            return children[0]->find_top_content_node();
        }

        return children[0]->find_top_content_node();
    }

    WMNode* find_bottom_content_node()
    {
        if (layout == NORMAL) {
            return this;
        }

        if (layout != HSPLIT) {
            return children[0]->find_bottom_content_node();
        }

        return children[1]->find_bottom_content_node();
    }

    WMNode* find_right_adjacent_node()
    {
        if (!parent) {
            return nullptr;
        }

        if (this == parent->get_left_child()) {
            return parent->get_right_child()->find_left_content_node();
        }

        return parent->find_right_adjacent_node();
    }

    WMNode* find_left_adjacent_node()
    {
        if (!parent) {
            return nullptr;
        }

        if (this == parent->get_right_child()) {
            return parent->get_left_child()->find_right_content_node();
        }

        return parent->find_left_adjacent_node();
    }

    WMNode* find_top_adjacent_node()
    {
        if (!parent) {
            return nullptr;
        }

        if (this == parent->get_bottom_child()) {
            return parent->get_top_child()->find_bottom_content_node();
        }

        return parent->find_top_adjacent_node();
    }

    WMNode* find_bottom_adjacent_node()
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
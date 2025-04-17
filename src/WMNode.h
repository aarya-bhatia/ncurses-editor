#pragma once

#include <vector>
#include <list>
#include "Dimension.h"
#include "Window.h"
#include "log.h"
#include "File.h"
#include "FileView.h"
#include "ViewFactory.h"

struct WMNode
{
    std::vector<WMNode*> children;
    Dimension bounds;
    WMNode* parent = nullptr;

    std::list<Window*> tabs;
    std::list<Window*>::iterator current_tab;

    enum Layout { NORMAL, HSPLIT, VSPLIT }layout = NORMAL;

    WMNode(Dimension bounds, WMNode* parent) : bounds(bounds), parent(parent) {
        current_tab = tabs.end();
    }

    ~WMNode()
    {
        for (Window* tab : tabs) {
            delete tab;
        }

        for (WMNode* child : children) {
            delete child;
        }
    }

    Window* get_current_tab_window() {
        if (current_tab != tabs.end()) {
            return *current_tab;
        }
        return nullptr;
    }

    void open_prev_tab() {
        if (tabs.empty()) return;
        if (current_tab == tabs.begin()) { log_info("no prev tab"); return; }
        current_tab = std::prev(current_tab);
        resize(bounds);
    }

    void open_next_tab() {
        if (tabs.empty()) return;
        if (std::next(current_tab) == tabs.end()) { log_info("no next tab"); return; }
        current_tab = std::next(current_tab);
        resize(bounds);
    }

    Window *open_tab(File* f)
    {
        return open_tab(ViewFactory::new_file_view(f, bounds));
    }

    Window *open_tab(Window* c)
    {
        if (current_tab != tabs.end()) {
            (*current_tab)->unfocus();
        }

        tabs.push_back(c);
        current_tab = std::prev(tabs.end());
        c->focus();
        log_info("Window content added to WMnode");
        return *current_tab;
    }

    bool split_allowed() { return bounds.width / 2 >= 1 && bounds.height / 2 >= 1; }

    void resize(Dimension d)
    {
        for (auto* child : children)
        {
            Dimension child_d = d;
            child_d.width *= d.width / bounds.width;
            child_d.height *= d.height / bounds.height;
            child->resize(child_d);
        }

        // note: resize other tabs when they are opened.
        if (current_tab != tabs.end()) {
            (*current_tab)->resize(d);
        }

        bounds = d;
    }

    void close_tab()
    {
        if (current_tab != tabs.end()) {
            log_info("tab closed");
            tabs.erase(current_tab);
        }
        current_tab = tabs.end();
    }

    // TODO: transfer this nodes tabs to a child?
    void splith()
    {
        if (!split_allowed()) { return; }

        layout = HSPLIT;

        Dimension d1(bounds.x, bounds.y, bounds.width, bounds.height / 2);
        Dimension d2(bounds.x, bounds.y + bounds.height / 2, bounds.width, bounds.height / 2);

        WMNode* child1 = new WMNode(d1, this);
        WMNode* child2 = new WMNode(d2, this);

        children.push_back(child1);
        children.push_back(child2);

        if (current_tab != tabs.end()) {
            child1->open_tab((*current_tab)->copy(d1));
            child2->open_tab((*current_tab)->copy(d2));
            close_tab();
        }

        log_info("horizontal split complete");
    }

    void splitv()
    {
        if (!split_allowed()) { return; }

        layout = VSPLIT;

        Dimension d1(bounds.x, bounds.y, bounds.width / 2, bounds.height);
        Dimension d2(bounds.x + bounds.width / 2, bounds.y, bounds.width / 2, bounds.height);

        WMNode* child1 = new WMNode(d1, this);
        WMNode* child2 = new WMNode(d2, this);

        children.push_back(child1);
        children.push_back(child2);

        if (current_tab != tabs.end()) {
            child1->open_tab((*current_tab)->copy(d1));
            child2->open_tab((*current_tab)->copy(d2));
            close_tab();
        }

        log_info("horizontal split complete");
    }

    void draw()
    {
        for (auto* child : children) { child->draw(); }
        if (current_tab != tabs.end()) {
            (*current_tab)->draw();
        }
    }

    void show()
    {
        for (auto* child : children) { child->show(); }
        if (current_tab != tabs.end()) {
            (*current_tab)->show();
        }
    }

    int count_nodes()
    {
        int ans = 1;
        for (auto* child : children) { ans += child->count_nodes(); }
        return ans;
    }

    WMNode* get_left_child()
    {
        if (layout != VSPLIT) {
            return nullptr;
        }

        return children[0];
    }

    WMNode* get_right_child()
    {
        if (layout != VSPLIT) {
            return nullptr;
        }

        return children[1];
    }

    WMNode* get_top_child()
    {
        if (layout != HSPLIT) {
            return nullptr;
        }

        return children[0];
    }

    WMNode* get_bottom_child()
    {
        if (layout != HSPLIT) {
            return nullptr;
        }

        return children[1];
    }

    WMNode* find_left_content_node()
    {
        if (layout == NORMAL) {
            return this;
        }

        if (layout != VSPLIT) {
            return nullptr;
        }

        return children[0]->find_left_content_node();
    }

    WMNode* find_right_content_node()
    {
        if (layout == NORMAL) {
            return this;
        }

        if (layout != VSPLIT) {
            return nullptr;
        }

        return children[1]->find_right_content_node();
    }

    WMNode* find_top_content_node()
    {
        if (layout == NORMAL) {
            return this;
        }

        if (layout != HSPLIT) {
            return nullptr;
        }

        return children[0]->find_top_content_node();
    }

    WMNode* find_bottom_content_node()
    {
        if (layout == NORMAL) {
            return this;
        }

        if (layout != HSPLIT) {
            return nullptr;
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
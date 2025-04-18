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

struct WMNode : public IDrawable, IFocusable
{
    std::vector<WMNode*> children;
    Dimension bounds;
    WMNode* parent = nullptr;

    std::list<Window*> tabs;
    std::list<Window*>::iterator current_tab;

    bool focused = false;

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
        open_tab(std::prev(current_tab));
    }

    void open_next_tab() {
        if (tabs.empty()) return;
        if (std::next(current_tab) == tabs.end()) { log_info("no next tab"); return; }
        open_tab(std::next(current_tab));
    }

    Window* open_tab(File* f)
    {
        Window* tab_window = ViewFactory::new_file_view(f, bounds);
        tabs.push_back(tab_window);
        return open_tab(std::prev(tabs.end()));
    }

    Window* open_tab(std::list<Window*>::iterator tab);

    bool split_allowed() { return bounds.width / 2 >= 1 && bounds.height / 2 >= 1; }

    void resize(Dimension d) override;

    void close_tab();

    // TODO: transfer this nodes tabs to a child?
    void splith();
    void splitv();

    void draw() override
    {
        for (auto* child : children) { child->draw(); }

        if (current_tab != tabs.end()) {
            (*current_tab)->draw();
        }
    }

    void show() override
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
        if (layout == VSPLIT) return children[0];
        // log_debug("no left child of layout %d", layout);
        return nullptr;
    }

    WMNode* get_right_child()
    {
        if (layout == VSPLIT) return children[1];
        // log_debug("no right child of layout %d", layout);
        return nullptr;
    }

    WMNode* get_top_child()
    {
        if (layout == HSPLIT) return children[0];
        // log_debug("no top child of layout %d", layout);
        return nullptr;
    }

    WMNode* get_bottom_child()
    {
        if (layout == HSPLIT) return children[1];
        // log_debug("no bottom child of layout %d", layout);
        return nullptr;
    }

    WMNode* find_left_content_node()
    {
        // log_debug("finding left content node in WMNode %s", bounds.debug_string().c_str());

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
        // log_debug("finding right content node in WMNode %s", bounds.debug_string().c_str());

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
        // log_debug("finding top content node in WMNode %s", bounds.debug_string().c_str());

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
        // log_debug("finding bottom content node in WMNode %s", bounds.debug_string().c_str());

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
        // log_debug("finding right adjacent node of WMNode %s", bounds.debug_string().c_str());
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
        // log_debug("finding left adjacent node of WMNode %s", bounds.debug_string().c_str());
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
        // log_debug("finding top adjacent node of WMNode %s", bounds.debug_string().c_str());
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
        // log_debug("finding bottom adjacent node of WMNode %s", bounds.debug_string().c_str());
        if (!parent) {
            return nullptr;
        }

        if (this == parent->get_top_child()) {
            return parent->get_bottom_child()->find_top_content_node();
        }

        return parent->find_bottom_adjacent_node();
    }

    void focus() override {
        log_debug("got focus on WMNode %s", bounds.debug_string().c_str());
        focused = true;
        if (current_tab != tabs.end()) {
            (*current_tab)->focus();
        }
    }

    void unfocus() override {
        log_debug("lost focus on WMNode %s", bounds.debug_string().c_str());
        focused = false;
        if (current_tab != tabs.end()) {
            (*current_tab)->unfocus();
        }
    }

    void partial_draw_character(Cursor position) override {
        if (current_tab != tabs.end()) {
            (*current_tab)->partial_draw_character(position);
        }
    }

    void partial_draw_line(Cursor position) override {
        if (current_tab != tabs.end()) {
            (*current_tab)->partial_draw_line(position);
        }
    }

    void force_redraw() override {
        if (current_tab != tabs.end()) {
            (*current_tab)->force_redraw();
        }
    }
};
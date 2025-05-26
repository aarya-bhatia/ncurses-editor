#pragma once

#include <vector>
#include <array>
#include <list>
#include "Dimension.h"
#include "log.h"
#include "Window.h"

#include "ViewContainer.h"

struct WindowNode
{
    Dimension bounds;
    std::array<WindowNode*, 2> children = { nullptr, nullptr };
    WindowNode* parent = nullptr;
    enum Layout { NORMAL, HSPLIT, VSPLIT }layout = NORMAL;
    bool focused = false;
    Window* content = nullptr;

    WindowNode(Dimension bounds, WindowNode* parent = nullptr) : bounds(bounds), parent(parent) {
        log_debug("init window node %s", bounds.debug_string().c_str());
    }

    ~WindowNode()
    {
        for (WindowNode* child : children) {
            delete child;
        }

        delete content;
    }

    void set_content(Window* c) {
        delete content;
        content = c;
        content->resize(bounds);
        if (focused) content->focus();
    }

    Window* get_content() { return content; }

    WindowNode* sibling() const {
        if (!parent) {
            return nullptr;
        }

        if (parent->children[0] == this) return parent->children[1];
        if (parent->children[1] == this) return parent->children[0];

        return nullptr;
    }

    void focus() { focused = true; if (content)content->focus(); }
    void unfocus() { focused = false; if (content)content->unfocus(); }

    bool has_children() const { return children[0] != nullptr && children[1] != nullptr; }

    bool splitv_allowed(int min_width = 3) {
        if (bounds.width / 2 < min_width) {
            return false;
        }

        if (!has_children()) {
            return true;
        }

        for (WindowNode* child : children) {
            if (!child->splitv_allowed()) {
                return false;
            }
        }

        return true;
    }

    bool splith_allowed(int min_height = 3) {
        if (bounds.height / 2 < min_height) {
            return false;
        }

        if (!has_children()) {
            return true;
        }

        for (WindowNode* child : children) {
            if (!child->splith_allowed()) {
                return false;
            }
        }

        return true;
    }

    void get_hsplit_halves(Dimension& d1, Dimension& d2) {
        d1 = Dimension(bounds.x, bounds.y, bounds.width, bounds.height / 2);
        d2 = Dimension(bounds.x, bounds.y + bounds.height / 2, bounds.width, bounds.height - bounds.height / 2);
    }

    void get_vsplit_halves(Dimension& d1, Dimension& d2) {
        d1 = Dimension(bounds.x, bounds.y, bounds.width / 2, bounds.height);
        d2 = Dimension(bounds.x + bounds.width / 2, bounds.y, bounds.width - bounds.width / 2, bounds.height);
    }

    void resize(Dimension d)
    {
        log_info("Resized node %p to %s", this, d.debug_string().c_str());
        bounds = d;

        if (layout == NORMAL && content != nullptr) {
            content->resize(d);
            return;
        }

        Dimension d1, d2;

        if (layout == VSPLIT) {
            get_vsplit_halves(d1, d2);
            children[0]->resize(d1);
            children[1]->resize(d2);
        }
        else if (layout == HSPLIT) {
            get_hsplit_halves(d1, d2);
            children[0]->resize(d1);
            children[1]->resize(d2);
        }
    }

    void close_tab();

    void splith()
    {
        if (!splith_allowed()) { return; }

        layout = HSPLIT;

        Dimension d1, d2;
        get_hsplit_halves(d1, d2);

        log_debug("Split node %s to nodes %s and %s",
            bounds.debug_string().c_str(),
            d1.debug_string().c_str(),
            d2.debug_string().c_str());

        children[0] = new WindowNode(d1, this);
        children[1] = new WindowNode(d2, this);

        children[0]->content = this->content;
        this->content = nullptr;

        resize(bounds);
    }

    void splitv()
    {
        if (!splitv_allowed()) { return; }

        layout = VSPLIT;

        Dimension d1, d2;
        get_vsplit_halves(d1, d2);

        log_debug("Split node %s to nodes %s and %s",
            bounds.debug_string().c_str(),
            d1.debug_string().c_str(),
            d2.debug_string().c_str());

        children[0] = new WindowNode(d1, this);
        children[1] = new WindowNode(d2, this);

        children[0]->content = this->content;
        this->content = nullptr;

        resize(bounds);
    }

    void draw()
    {
        if (content) {
            content->draw();
        }

        if (!has_children()) {
            return;
        }

        for (WindowNode* child : children) { child->draw(); }
    }

    int count_nodes()
    {
        int ans = 1;

        if (!has_children()) {
            return ans;
        }

        for (WindowNode* child : children) { ans += child->count_nodes(); }
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

    WindowNode* find_left_content_node(WindowNode* orig_content_node)
    {
        switch (layout)
        {
        case NORMAL: return this;
        case VSPLIT: return get_left_child()->find_left_content_node(orig_content_node);
        case HSPLIT: return find_nearest_child(orig_content_node)->find_left_content_node(orig_content_node);
        default: return nullptr;
        }
    }

    WindowNode* find_right_content_node(WindowNode* orig_content_node)
    {
        switch (layout)
        {
        case NORMAL: return this;
        case VSPLIT: return get_right_child()->find_right_content_node(orig_content_node);
        case HSPLIT: return find_nearest_child(orig_content_node)->find_right_content_node(orig_content_node);
        default: return nullptr;
        }
    }

    WindowNode* find_top_content_node(WindowNode* orig_content_node = nullptr)
    {
        switch (layout)
        {
        case NORMAL: return this;
        case HSPLIT: return get_top_child()->find_top_content_node(orig_content_node);
        case VSPLIT: return find_nearest_child(orig_content_node)->find_top_content_node(orig_content_node);
        default: return nullptr;
        }
    }

    WindowNode* find_bottom_content_node(WindowNode* orig_content_node)
    {
        switch (layout)
        {
        case NORMAL: return this;
        case HSPLIT: return get_bottom_child()->find_bottom_content_node(orig_content_node);
        case VSPLIT: return find_nearest_child(orig_content_node)->find_bottom_content_node(orig_content_node);
        default: return nullptr;
        }
    }

    WindowNode* find_nearest_child(WindowNode* orig_content_node) {
        if (!orig_content_node || layout == NORMAL || children.empty()) {
            return nullptr;
        }

        Point orig_center = orig_content_node->bounds.center();
        Point first_child_center = children[0]->bounds.center();
        Point second_child_center = children[1]->bounds.center();
        float from_first_child = orig_center.distance_squared(first_child_center);
        float from_second_child = orig_center.distance_squared(second_child_center);

        return (from_first_child <= from_second_child) ? children[0] : children[1];
    }

    WindowNode* find_right_adjacent_node(WindowNode* orig_content_node = nullptr)
    {
        if (!parent) {
            return nullptr;
        }

        if (!orig_content_node) orig_content_node = this;

        if (this == parent->get_left_child()) {
            return parent->get_right_child()->find_left_content_node(orig_content_node);
        }

        return parent->find_right_adjacent_node(orig_content_node);
    }

    WindowNode* find_left_adjacent_node(WindowNode* orig_content_node = nullptr)
    {
        if (!parent) {
            return nullptr;
        }

        if (!orig_content_node) orig_content_node = this;


        if (this == parent->get_right_child()) {
            return parent->get_left_child()->find_right_content_node(orig_content_node);
        }

        return parent->find_left_adjacent_node(orig_content_node);
    }

    WindowNode* find_top_adjacent_node(WindowNode* orig_content_node = nullptr)
    {
        if (!parent) {
            return nullptr;
        }

        if (!orig_content_node) orig_content_node = this;


        if (this == parent->get_bottom_child()) {
            return parent->get_top_child()->find_bottom_content_node(orig_content_node);
        }

        return parent->find_top_adjacent_node(orig_content_node);
    }

    WindowNode* find_bottom_adjacent_node(WindowNode* orig_content_node = nullptr)
    {
        if (!parent) {
            return nullptr;
        }

        if (!orig_content_node) orig_content_node = this;

        if (this == parent->get_top_child()) {
            return parent->get_bottom_child()->find_top_content_node(orig_content_node);
        }

        return parent->find_bottom_adjacent_node(orig_content_node);
    }
};
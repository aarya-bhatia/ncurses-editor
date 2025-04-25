#pragma once

#include <vector>
#include <list>
#include "Dimension.h"
#include "log.h"

template<typename T>
struct WindowNode
{
    Dimension bounds;
    std::vector<WindowNode<T>*> children;
    WindowNode<T>* parent = nullptr;
    enum Layout { NORMAL, HSPLIT, VSPLIT }layout = NORMAL;

    T content;

    WindowNode(Dimension bounds, WindowNode<T>* parent = nullptr) : bounds(bounds), parent(parent) {}

    ~WindowNode()
    {
        for (WindowNode* child : children) {
            delete child;
        }
    }

    bool splitv_allowed() { return bounds.width / 2 >= 3; }
    bool splith_allowed() { return bounds.height / 2 >= 3; }

    void resize(Dimension d)
    {
        for (auto* child : children)
        {
            Dimension child_d = d;
            child_d.width *= d.width / bounds.width;
            child_d.height *= d.height / bounds.height;
            child->resize(child_d);
        }

        if (children.empty()) {
            content.resize(d);
        }

        bounds = d;
    }

    void close_tab();

    // TODO: transfer this nodes tab_window to a child?
    void splith();
    void splitv();

    void draw()
    {
        if (content) {
            content.draw();
        }

        for (auto* child : children) { child->draw(); }
    }

    void show()
    {
        if (content) {
            content.show();
        }

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
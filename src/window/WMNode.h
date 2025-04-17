#pragma once

#include <vector>
#include "Dimension.h"
#include "Window.h"
#include "log.h"

struct WMNode
{
    std::vector<WMNode*> children;
    Dimension bounds;
    WMNode* parent = nullptr;
    Window* content = nullptr;
    enum Layout { NORMAL, HSPLIT, VSPLIT }layout = NORMAL;

    WMNode(Dimension bounds, WMNode* parent) : bounds(bounds), parent(parent) {}

    void set_content(Window* c)
    {
        if (content) {
            content->unfocus();
        }

        content = c;
        content->focus();
        log_info("Window content added to WMnode");
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

        if (content) {
            content->resize(d);
        }

        bounds = d;
    }

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

        if (content) {
            child1->content = content->copy(d1);
            child2->content = content->copy(d2);
            delete content; content = nullptr;
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

        if (content) {
            child1->content = content->copy(d1);
            child2->content = content->copy(d2);
            delete content; content = nullptr;
        }

        log_info("horizontal split complete");
    }

    void draw()
    {
        for (auto* child : children) { child->draw(); }
        if (content) {
            content->draw();
        }
    }

    void show()
    {
        for (auto* child : children) { child->show(); }
        if (content) {
            content->show();
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
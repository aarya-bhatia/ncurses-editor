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

    T content = nullptr;

    WindowNode(Dimension bounds, WindowNode<T>* parent = nullptr) : bounds(bounds), parent(parent) {
        log_debug("init window node %s", bounds.debug_string().c_str());
    }

    ~WindowNode()
    {
        for (WindowNode<T>* child : children) {
            delete child;
        }
    }

    void focus() { if (content)content->focus(); }
    void unfocus() { if (content)content->unfocus(); }

    bool splitv_allowed(int min_width = 3) {
        if (bounds.width / 2 < min_width) {
            return false;
        }

        for (WindowNode<T>* child : children) {
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

        for (WindowNode<T>* child : children) {
            if (!child->splith_allowed()) {
                return false;
            }
        }

        return true;
    }

    void resize(Dimension d)
    {
        log_info("resize node %s", d.debug_string().c_str());
        for (WindowNode<T>* child : children)
        {
            Dimension child_d = child->bounds;
            child_d.width *= d.width / bounds.width;
            child_d.height *= d.height / bounds.height;
            child->resize(child_d);
        }

        if (children.empty() && content != nullptr) {
            content->resize(d);
        }

        bounds = d;
    }

    void close_tab();

    void splith()
    {
        if (!splith_allowed()) { return; }

        layout = HSPLIT;

        Dimension d1(bounds.x, bounds.y, bounds.width, bounds.height / 2);
        Dimension d2(bounds.x, bounds.y + bounds.height / 2, bounds.width, bounds.height / 2);

        log_info("Split node %s to nodes %s and %s",
            bounds.debug_string().c_str(),
            d1.debug_string().c_str(),
            d2.debug_string().c_str());

        WindowNode<T>* child1 = new WindowNode<T>(d1, this);
        WindowNode<T>* child2 = new WindowNode<T>(d2, this);

        children.push_back(child1);
        children.push_back(child2);

        if (this->content) {
            this->content->clear();
            this->content->resize(d1);
        }

        child1->content = this->content;
        this->content = nullptr;
    }

    void splitv()
    {
        if (!splitv_allowed()) { return; }

        layout = VSPLIT;

        Dimension d1(bounds.x, bounds.y, bounds.width / 2, bounds.height);
        Dimension d2(bounds.x + bounds.width / 2, bounds.y, bounds.width / 2, bounds.height);

        log_info("Split node %s to nodes %s and %s",
            bounds.debug_string().c_str(),
            d1.debug_string().c_str(),
            d2.debug_string().c_str());

        WindowNode<T>* child1 = new WindowNode<T>(d1, this);
        WindowNode<T>* child2 = new WindowNode<T>(d2, this);

        children.push_back(child1);
        children.push_back(child2);

        if (this->content) {
            this->content->clear();
            this->content->resize(d1);
        }

        child1->content = this->content;
        this->content = nullptr;
    }

    void draw()
    {
        if (content) {
            content->draw();
        }

        for (WindowNode<T>* child : children) { child->draw(); }
    }

    void show()
    {
        if (content) {
            content->show();
        }

        for (WindowNode<T>* child : children) { child->show(); }
    }

    int count_nodes()
    {
        int ans = 1;
        for (WindowNode<T>* child : children) { ans += child->count_nodes(); }
        return ans;
    }

    WindowNode<T>* get_left_child()
    {
        if (layout == VSPLIT) return children[0];
        return nullptr;
    }

    WindowNode<T>* get_right_child()
    {
        if (layout == VSPLIT) return children[1];
        return nullptr;
    }

    WindowNode<T>* get_top_child()
    {
        if (layout == HSPLIT) return children[0];
        return nullptr;
    }

    WindowNode<T>* get_bottom_child()
    {
        if (layout == HSPLIT) return children[1];
        return nullptr;
    }

    WindowNode<T>* find_left_content_node(WindowNode<T>* orig_content_node)
    {
        switch (layout)
        {
        case NORMAL: return this;
        case VSPLIT: return get_left_child()->find_left_content_node(orig_content_node);
        case HSPLIT: return find_nearest_child(orig_content_node)->find_left_content_node(orig_content_node);
        default: return nullptr;
        }
    }

    WindowNode<T>* find_right_content_node(WindowNode<T>* orig_content_node)
    {
        switch (layout)
        {
        case NORMAL: return this;
        case VSPLIT: return get_right_child()->find_right_content_node(orig_content_node);
        case HSPLIT: return find_nearest_child(orig_content_node)->find_right_content_node(orig_content_node);
        default: return nullptr;
        }
    }

    WindowNode<T>* find_top_content_node(WindowNode<T>* orig_content_node = nullptr)
    {
        switch (layout)
        {
        case NORMAL: return this;
        case HSPLIT: return get_top_child()->find_top_content_node(orig_content_node);
        case VSPLIT: return find_nearest_child(orig_content_node)->find_top_content_node(orig_content_node);
        default: return nullptr;
        }
    }

    WindowNode<T>* find_bottom_content_node(WindowNode<T>* orig_content_node)
    {
        switch (layout)
        {
        case NORMAL: return this;
        case HSPLIT: return get_bottom_child()->find_bottom_content_node(orig_content_node);
        case VSPLIT: return find_nearest_child(orig_content_node)->find_bottom_content_node(orig_content_node);
        default: return nullptr;
        }
    }

    WindowNode<T>* find_nearest_child(WindowNode<T>* orig_content_node) {
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

    WindowNode<T>* find_right_adjacent_node(WindowNode<T>* orig_content_node = nullptr)
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

    WindowNode<T>* find_left_adjacent_node(WindowNode<T>* orig_content_node = nullptr)
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

    WindowNode<T>* find_top_adjacent_node(WindowNode<T>* orig_content_node = nullptr)
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

    WindowNode<T>* find_bottom_adjacent_node(WindowNode<T>* orig_content_node = nullptr)
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
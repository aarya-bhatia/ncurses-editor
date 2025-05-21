#pragma once

#include "WindowNode.h"
#include <assert.h>

class WindowTab
{
private:
    WindowNode* _root_node = NULL;
    WindowNode* _focused_node = NULL;
    Dimension _bounds;

public:
    struct Visitor {
        virtual ~Visitor() = default;
        virtual void operator()(Window* content) = 0;
    };

    WindowTab(Dimension d) : _bounds(d) { _init(); }

    ~WindowTab() { _destroy(); }

    WindowNode* get_focused_node() { return _focused_node; }

    Dimension get_bounds() const { return _bounds; }
    Dimension get_focused_bounds() const { return _focused_node->bounds; }

    void accept(Visitor* v) {
        if (v) _accept(_root_node, v);
    }

    void redraw() {
        _root_node->redraw();
    }

    void draw() {
        _root_node->draw();
    }

    void show() {
        _root_node->show();
    }

    void resize(Dimension d) {
        if (_bounds == d) { return; }
        _bounds = d;
        _root_node->resize(d);
    }

    bool splith() {
        if (!_focused_node->splith_allowed()) { return false; }
        _focused_node->splith();
        assert(_focused_node->layout == WindowNode::Layout::HSPLIT);
        _set_focused_node(_focused_node->get_top_child());
        return true;
    }

    bool splitv() {
        if (!_focused_node->splitv_allowed()) { return false; }
        _focused_node->splitv();
        assert(_focused_node->layout == WindowNode::Layout::VSPLIT);
        _set_focused_node(_focused_node->get_left_child());
        return true;
    }

    bool focus_right() {
        WindowNode* new_node = _focused_node->find_right_adjacent_node(_focused_node);
        if (!new_node) {
            return false;
        }
        assert(new_node->layout == WindowNode::Layout::NORMAL);
        _set_focused_node(new_node);
        return true;
    }

    bool focus_left() {
        WindowNode* new_node = _focused_node->find_left_adjacent_node(_focused_node);
        if (!new_node) {
            return false;
        }
        assert(new_node->layout == WindowNode::Layout::NORMAL);
        _set_focused_node(new_node);
        return true;
    }

    bool focus_top() {
        WindowNode* new_node = _focused_node->find_top_adjacent_node(_focused_node);
        if (!new_node) {
            return false;
        }
        assert(new_node->layout == WindowNode::Layout::NORMAL);
        _set_focused_node(new_node);
        return true;
    }

    bool focus_bottom() {
        WindowNode* new_node = _focused_node->find_bottom_adjacent_node(_focused_node);
        if (!new_node) {
            return false;
        }
        assert(new_node->layout == WindowNode::Layout::NORMAL);
        _set_focused_node(new_node);
        return true;
    }

private:

    void _set_focused_node(WindowNode* node) {
        assert(_focused_node);
        assert(node);
        _focused_node->unfocus();
        _focused_node = node;
        _focused_node->focus();
    }

    void _destroy() {
        log_debug("destroying window tab %p", this);
        delete _root_node;
        _focused_node = _root_node = nullptr;
    }

    void _init() {
        if (_focused_node) { return; }
        log_debug("init window tab %p", this);
        _root_node = new WindowNode(_bounds, nullptr);
        _root_node->bounds = _bounds;
        _focused_node = _root_node;
        _focused_node->focus();
    }

    void _accept(WindowNode* root, Visitor* v) {
        if (!root) {
            return;
        }

        for (WindowNode* child : root->children) {
            _accept(child, v);
        }

        if (root->content) (*v)(root->content);
    }
};
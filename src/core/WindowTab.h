#pragma once

#include "WindowNode.h"
#include <assert.h>
#include <functional>
#include "FileViewFactory.h"

class WindowTab
{
private:
    WindowNode* _root_node = NULL;
    WindowNode* _focused_node = NULL;
    Dimension _bounds;

public:
    WindowTab(Dimension d) : _bounds(d) { _init(); }

    ~WindowTab() { _destroy(); }

    Window* get_focused_content() { return _focused_node->content; }
    void set_focused_content(Window* content) { _focused_node->set_content(content); }

    WindowNode* get_focused_node() { return _focused_node; }

    Dimension get_bounds() const { return _bounds; }
    Dimension get_focused_bounds() const { return _focused_node->bounds; }

    void close_focused_node() {
        if (_root_node == _focused_node) {
            delete _root_node;
            _root_node = _focused_node = nullptr;
            _init();
            return;
        }

        WindowNode* grandparent = _focused_node->parent->parent;
        WindowNode* sibling = _focused_node->sibling();
        WindowNode* parent = _focused_node->parent;
        WindowNode* next_focused_node = sibling->find_first_content_node();

        _focused_node->parent->children[0] = nullptr;
        _focused_node->parent->children[1] = nullptr;

        if (!grandparent) {
            assert(_root_node == parent);
            _root_node = sibling;
            sibling->parent = nullptr;
            _root_node->resize(_bounds);
        }
        else {
            sibling->parent = grandparent;

            if (grandparent->children[0] == parent) {
                grandparent->children[0] = sibling;
            }
            else {
                grandparent->children[1] = sibling;
            }

            grandparent->resize(grandparent->bounds);
        }

        delete parent;
        delete _focused_node;

        _focused_node = next_focused_node;
        _focused_node->focus();
    }

    void accept(std::function<void(Window*)> cb) {
        if (cb != nullptr) {
            _accept(_root_node, cb);
        }
    }

    void draw() {
        _root_node->draw();
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
        _focused_node->sibling()->set_content(FileViewFactory::create_content_window(_focused_node->sibling()->bounds));
        return true;
    }

    bool splitv() {
        if (!_focused_node->splitv_allowed()) { return false; }
        _focused_node->splitv();
        assert(_focused_node->layout == WindowNode::Layout::VSPLIT);
        _set_focused_node(_focused_node->get_left_child());
        _focused_node->sibling()->set_content(FileViewFactory::create_content_window(_focused_node->sibling()->bounds));
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

    void hide() {
        _root_node->hide();
    }

    void show() {
        _root_node->show();
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
        log_debug("init window tab %p with bounds %s", this, _bounds.debug_string().c_str());
        _root_node = new WindowNode(_bounds, nullptr);
        _root_node->bounds = _bounds;
        _focused_node = _root_node;
        _focused_node->focus();
        _focused_node->set_content(FileViewFactory::create_content_window(_bounds));
    }

    void _accept(WindowNode* root, std::function<void(Window*)> cb) {
        if (!root) {
            return;
        }

        for (WindowNode* child : root->children) {
            _accept(child, cb);
        }

        cb(root->content);
    }
};
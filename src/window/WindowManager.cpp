#include "log.h"
#include "WindowManager.h"
#include "HSplitContainerWindow.h"
#include "VSplitContainerWindow.h"

void WindowManager::focus(ContentWindow* node)
{
    assert(node);
    if (current_node) {
        current_node->on_unfocus();
    }
    current_node = node;
    current_node->on_focus();
}

void WindowManager::unfocus(ContentWindow* node)
{
    assert(node);
    if (current_node == node) {
        current_node->on_unfocus();
        current_node = nullptr;
    }
}

void WindowManager::set_content(ContentWindow* content_window) {
    log_info("setting new window content node");

    assert(content_window);
    assert(content_window->get_content());

    if (current_node) {
        current_node->on_unfocus();
    }

    if (!root_node) {
        log_debug("root node added");
        assert(!current_node);
        root_node = content_window;
    }
    else if (current_node == root_node) {
        log_debug("new root node");
        root_node = content_window;
    }
    else {
        log_debug("swapping current node with new content");
        assert(current_node->parent);
        current_node->parent->swap_child(current_node, content_window);
    }

    if (content_window->parent) {
        assert(content_window != root_node);
        content_window->parent->arrange_children();
    }
    else {
        assert(content_window == root_node);
        content_window->resize(screen_bounds);
    }

    assert(root_node);
    delete current_node; current_node = content_window;
    current_node->on_focus();
}

ContentWindow* WindowManager::get_content_node() {
    if (current_node) {
        return current_node->get_content();
    }

    if (root_node) {
        if ((current_node = _find_content_node(root_node)) != nullptr) {
            return current_node->get_content();
        }
    }

    log_info("No content node");
    return nullptr;
}

ContentWindow* WindowManager::_find_content_node(Window* node)
{
    if (node->get_content()) {
        return node->get_content();
    }

    assert(node->get_container());

    for (auto child : node->get_container()->get_children()) {
        ContentWindow* result = _find_content_node(child);
        if (result) {
            return result;
        }
    }

    return nullptr;
}


void WindowManager::_split(ContainerWindow* orig_parent, ContainerWindow* split_container, ContentWindow* new_content) {
    assert(split_container);

    if (orig_parent) {
        log_info("splitting current node");
        orig_parent->swap_child(current_node, split_container);
        assert(split_container->parent == orig_parent);
    }
    else {
        log_info("splitting root node");
        root_node = split_container;
    }

    split_container->add_child(current_node);
    split_container->add_child(new_content);
    split_container->arrange_children();

    assert(current_node->parent == split_container);
    assert(split_container->count_children() >= 2);
    assert(split_container->get_children().front() == current_node);
}

bool WindowManager::split_horizontal(ContentWindow* new_content) {
    if (!current_node || !current_node->get_content()) {
        return false;
    }

    ContainerWindow* container = new HSplitContainerWindow(current_node->bounds);
    _split(current_node->parent, container, new_content);
    return true;
}

bool WindowManager::split_vertical(ContentWindow* new_content) {
    if (!current_node || !current_node->get_content()) {
        return false;
    }

    ContainerWindow* container = new VSplitContainerWindow(current_node->bounds);
    _split(current_node->parent, container, new_content);
    return true;
}

ContentWindow* WindowManager::get_content_node_right(Window* current)
{
    if (!current->parent) {
        return nullptr;
    }

    Window* child = current->parent->get_right_child(current);
    if (child) {
        return child->get_left_most_content_node();
    }

    return get_content_node_right(current->parent);
}

ContentWindow* WindowManager::get_content_node_left(Window* current)
{
    if (!current->parent) {
        return nullptr;
    }

    Window* child = current->parent->get_left_child(current);
    if (child) {
        return child->get_right_most_content_node();
    }

    return get_content_node_left(current->parent);
}


ContentWindow* WindowManager::get_content_node_top(Window* current)
{
    if (!current->parent) {
        return nullptr;
    }

    Window* child = current->parent->get_top_child(current);
    if (child) {
        return child->get_bottom_most_content_node();
    }

    return get_content_node_top(current->parent);
}

ContentWindow* WindowManager::get_content_node_bottom(Window* current)
{
    if (!current->parent) {
        return nullptr;
    }

    Window* child = current->parent->get_bottom_child(current);
    if (child) {
        return child->get_top_most_content_node();
    }

    return get_content_node_bottom(current->parent);
}
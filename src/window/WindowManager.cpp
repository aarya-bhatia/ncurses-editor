#include "log.h"
#include "WindowManager.h"

void WindowManager::set_content(ContentWindow* content_window) {
    log_info("setting new window content node");

    assert(content_window);
    assert(content_window->get_content());

    if (!root_node) {
        log_debug("root node added");
        assert(!current_node);
        current_node = root_node = content_window;
    }
    else if (current_node == root_node) {
        log_debug("new root node");
        ContentWindow* tmp = current_node->get_content();
        assert(tmp);
        current_node = root_node = content_window;
        delete tmp;
    }
    else {
        log_debug("swapping current node with new content");
        assert(current_node->parent);
        ContentWindow* tmp = current_node->get_content();
        assert(tmp);
        current_node->parent->swap_child(current_node, content_window);
        delete tmp;
    }

    if (content_window->parent) {
        content_window->parent->arrange_children();
    }
    else {
        content_window->resize(screen_bounds);
    }

    assert(root_node);
    current_node = content_window;
}

ContentWindow* WindowManager::get_content_node() {
    if (current_node) {
        return current_node->get_content();
    }

    if (root_node) {
        return _find_content_node(root_node);
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

    for (auto child : node->get_container()->children) {
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
}

bool WindowManager::split_horizontal(ContentWindow* new_content) {
    if (!current_node) {
        return false;
    }

    ContainerWindow* container = new HSplitContainerWindow(current_node->bounds);
    _split(current_node->parent, container, new_content);
    return true;
}

bool WindowManager::split_vertical(ContentWindow* new_content) {
    if (!current_node) {
        return false;
    }

    ContainerWindow* container = new VSplitContainerWindow(current_node->bounds);
    _split(current_node->parent, container, new_content);
    return true;
}
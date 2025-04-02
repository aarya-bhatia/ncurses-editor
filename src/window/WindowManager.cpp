#include "log.h"
#include "WindowManager.h"

WindowManager::WindowManager(Dimension bounds) : screen_bounds(bounds) {
}

WindowManager::~WindowManager() {
    delete root_node;
    root_node = current_node = nullptr;
}

Dimension WindowManager::get_bounds() const { return screen_bounds; }

void WindowManager::set_content(ContentWindow* content_window) {
    assert(content_window);
    assert(content_window->get_content());

    if (!root_node) {
        assert(!current_node);
        current_node = root_node = content_window;
    }
    else if (current_node == root_node) {
        ContentWindow* tmp = current_node->get_content();
        assert(tmp);
        current_node = root_node = content_window;
        delete tmp;
    }
    else {
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

void WindowManager::draw() {
    if (current_node) { current_node->draw(); }
}

void WindowManager::show() {
    if (current_node) { current_node->show(); }
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


    // void _split(ContainerWindow* orig_parent, ContainerWindow* split_container) {
    //     assert(split_container);

    //     if (orig_parent) {
    //         log_info("splitting current node");
    //         orig_parent->swap_child(current_node, split_container);
    //         assert(split_container->parent == orig_parent);
    //     }
    //     else {
    //         log_info("splitting root node");
    //         root_node = split_container;
    //     }

    //     split_container->add_child(current_node);
    //     split_container->add_child(new ContentWindow(*current_node->get_content()));
    //     split_container->arrange_children();
    // }

    // bool split_horizontal() {
    //     if (!current_node) {
    //         return false;
    //     }

    //     ContainerWindow* container = new HSplitContainerWindow(current_node->bounds);
    //     _split(current_node->parent, container);
    //     return true;
    // }

    // bool split_vertical() {
    //     if (!current_node) {
    //         return false;
    //     }

    //     ContainerWindow* container = new VSplitContainerWindow(current_node->bounds);
    //     _split(current_node->parent, container);
    //     return true;
    // }
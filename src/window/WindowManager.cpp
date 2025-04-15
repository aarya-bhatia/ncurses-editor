#include "log.h"
#include "WindowManager.h"
#include "HSplitContainerWindow.h"
#include "VSplitContainerWindow.h"
#include <list>

WindowManager::WindowManager(Dimension bounds) : screen_bounds(bounds), window_id_generator(1) {
}

WindowManager::~WindowManager() {
    delete root_node;
    root_node = current_node = nullptr;
}

bool WindowManager::resize(Dimension bounds) {
    if (root_node && root_node->resizable(bounds)) {
        root_node->resize(bounds);
        screen_bounds = bounds;
        return true;
    }

    return false;
}

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
    assert(content_window);
    assert(content_window->get_content());

    content_window->id = window_id_generator.next();

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
    log_debug("Added node %s", content_window->debug_string().c_str());
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


bool WindowManager::split_horizontal(ContentWindow* new_content) {
    assert(current_node);
    assert(new_content);

    // if (current_node) {//} && (current_node->bounds.width / 2 < Window::MIN_WINDOW_SIZE || current_node->bounds.height / 2 < Window::MIN_WINDOW_SIZE)) {
    //     log_warn("not resizable");
    //     return false;
    // }

    ContainerWindow* container = new HSplitContainerWindow(current_node->bounds);
    container->adopt_child(current_node);
    container->add_child(new_content);
    container->arrange_children();
    if (root_node == current_node) {
        root_node = container;
    }

    return true;
}

bool WindowManager::split_vertical(ContentWindow* new_content) {
    assert(current_node);
    assert(new_content);

    // if (current_node) {//} && (current_node->bounds.width / 2 < Window::MIN_WINDOW_SIZE || current_node->bounds.height / 2 < Window::MIN_WINDOW_SIZE)) {
    //     log_warn("not resizable");
    //     return false;
    //     }

    ContainerWindow* container = new VSplitContainerWindow(current_node->bounds);
    container->adopt_child(current_node);
    container->add_child(new_content);
    container->arrange_children();
    if (root_node == current_node) {
        root_node = container;
    }

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

int WindowManager::count_nodes() const
{
    int ans = 0;
    std::list<Window*> q;
    q.push_back(root_node);
    while (!q.empty())
    {
        Window* cur = q.front();
        q.pop_front();

        if (!cur) {
            continue;
        }

        ans += 1;
        if (cur->get_container()) {
            for (auto child : cur->get_container()->children) {
                q.push_back(child);
            }
        }
    }
    return ans;
}

int WindowManager::count_content_nodes() const
{
    int ans = 0;
    std::list<Window*> q;
    q.push_back(root_node);
    while (!q.empty())
    {
        Window* cur = q.front();
        q.pop_front();

        if (!cur) {
            continue;
        }

        if (cur->get_content()) {
            ans += 1;
        }

        if (cur->get_container()) {
            for (auto child : cur->get_container()->children) {
                q.push_back(child);
            }
        }
    }
    return ans;
}

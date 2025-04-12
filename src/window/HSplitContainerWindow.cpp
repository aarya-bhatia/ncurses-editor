#include "HSplitContainerWindow.h"

Window* HSplitContainerWindow::get_top_child(Window* child)
{
    if (children.size() < 2) {
        return nullptr;
    }

    return (children[1] == child) ? children[0] : nullptr;
}

Window* HSplitContainerWindow::get_bottom_child(Window* child)
{
    if (children.size() < 2) {
        return nullptr;
    }

    return (children[0] == child) ? children[1] : nullptr;
}

ContentWindow* HSplitContainerWindow::get_top_most_content_node() {
    for (auto child : children) {
        ContentWindow* w = child->get_top_most_content_node();
        if (w) {
            return w;
        }
    }
    return nullptr;
}

ContentWindow* HSplitContainerWindow::get_bottom_most_content_node() {
    for (auto it = children.rbegin(); it != children.rend(); ++it) {
        Window* child = *it;
        ContentWindow* w = child->get_bottom_most_content_node();
        if (w) {
            return w;
        }
    }
    return nullptr;

}
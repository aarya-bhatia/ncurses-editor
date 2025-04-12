#include "VSplitContainerWindow.h"

Window* VSplitContainerWindow::get_right_child(Window* child)
{
    if (children.size() < 2) {
        return nullptr;
    }

    return (children[0] == child) ? children[1] : nullptr;
}

Window* VSplitContainerWindow::get_left_child(Window* child)
{
    if (children.size() < 2) {
        return nullptr;
    }

    return (children[1] == child) ? children[0] : nullptr;
}

ContentWindow* VSplitContainerWindow::get_left_most_content_node() {
    for (auto child : children) {
        ContentWindow* w = child->get_left_most_content_node();
        if (w) {
            return w;
        }
    }
    return nullptr;
}

ContentWindow* VSplitContainerWindow::get_right_most_content_node() {
    for (auto it = children.rbegin(); it != children.rend(); ++it) {
        Window* child = *it;
        ContentWindow* w = child->get_right_most_content_node();
        if (w) {
            return w;
        }
    }
    return nullptr;

}
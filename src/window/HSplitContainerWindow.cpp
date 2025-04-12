#include "ContainerWindow.h"

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
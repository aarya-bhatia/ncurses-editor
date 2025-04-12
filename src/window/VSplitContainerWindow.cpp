#include "ContainerWindow.h"

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
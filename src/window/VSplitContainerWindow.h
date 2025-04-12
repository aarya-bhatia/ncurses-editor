#pragma once

#include "ContainerWindow.h"

struct VSplitContainerWindow : public ContainerWindow
{
    VSplitContainerWindow(Dimension bounds) :
        ContainerWindow(bounds, new VSplitResizeStrategy())
    {
    }

    Window* get_right_child(Window* child) override;
    Window* get_left_child(Window* child) override;

    ContentWindow* get_left_most_content_node() override;
    ContentWindow* get_right_most_content_node() override;
};
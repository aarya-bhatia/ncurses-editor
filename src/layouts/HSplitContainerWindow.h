#pragma once

#include "ContainerWindow.h"

struct HSplitContainerWindow : public ContainerWindow
{
    HSplitContainerWindow(Dimension bounds) :
        ContainerWindow(bounds, new HSplitResizeStrategy())
    {
    }

    Window* get_top_child(Window* child) override;
    Window* get_bottom_child(Window* child) override;

    ContentWindow* get_top_most_content_node() override;
    ContentWindow* get_bottom_most_content_node() override;

    std::string debug_string() const override {
        return "hsplit-" + ContainerWindow::debug_string();
    }
};

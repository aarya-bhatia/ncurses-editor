#pragma once
#include "Window.h"
#include "WindowResizeStrategies.h"
#include <vector>

struct ContainerWindow : public Window
{
    std::vector<Window*> children;
    WindowResizeStrategy* resize_strategy = nullptr;

    ContainerWindow(Dimension bounds, WindowResizeStrategy* resize_strategy);

    virtual ~ContainerWindow();

    size_t count_children() const;

    bool has_child(Window* child) const;

    void swap_child(Window* old_child, Window* new_child);

    void add_child(Window* child);

    void remove_child(Window* child);

    void arrange_children();

    ContainerWindow* get_container() override;

    void draw() override;

    void show() override;

    bool resize(Dimension new_bounds) override;
};

struct HSplitContainerWindow : public ContainerWindow
{
    HSplitContainerWindow(Dimension bounds) :
        ContainerWindow(bounds, new HSplitResizeStrategy())
    {
    }
};

struct VSplitContainerWindow : public ContainerWindow
{
    VSplitContainerWindow(Dimension bounds) :
        ContainerWindow(bounds, new VSplitResizeStrategy())
    {
    }
};
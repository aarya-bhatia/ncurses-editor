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

    std::vector<Window*>& get_children() { return children; }

    size_t count_children() const { return children.size(); }

    bool has_child(Window* child) const;

    void swap_child(Window* old_child, Window* new_child);

    void add_child(Window* child);

    void remove_child(Window* child);

    void arrange_children();

    ContainerWindow* get_container() override { return this; }

    virtual Window* get_right_child(Window* child) { return nullptr; }
    virtual Window* get_top_child(Window* child) { return nullptr; }
    virtual Window* get_bottom_child(Window* child) { return nullptr; }
    virtual Window* get_left_child(Window* child) { return nullptr; }

    void draw() override;
    void show() override;
};

struct HSplitContainerWindow : public ContainerWindow
{
    HSplitContainerWindow(Dimension bounds) :
        ContainerWindow(bounds, new HSplitResizeStrategy())
    {
    }

    Window* get_top_child(Window* child) override;
    Window* get_bottom_child(Window* child) override;
};

struct VSplitContainerWindow : public ContainerWindow
{
    VSplitContainerWindow(Dimension bounds) :
        ContainerWindow(bounds, new VSplitResizeStrategy())
    {
    }

    Window* get_right_child(Window* child) override;
    Window* get_left_child(Window* child) override;
};
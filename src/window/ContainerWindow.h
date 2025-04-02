#pragma once
#include "Window.h"
#include "WindowResizeStrategies.h"
#include <vector>

struct ContainerWindow : public Window
{
    std::vector<Window*> children;
    WindowResizeStrategy* resize_strategy = nullptr;

    ContainerWindow(Dimension bounds, WindowResizeStrategy* resize_strategy) :
        Window(bounds), resize_strategy(resize_strategy) {
        if (!resize_strategy) {
            resize_strategy = new ProportionalResizeStrategy;
        }
    }

    virtual ~ContainerWindow() {
        delete resize_strategy;
        resize_strategy = nullptr;

        for (auto child : children) {
            delete child;
        }

        children.clear();
    }

    size_t count_children() const {
        return children.size();
    }

    bool has_child(Window* child) const {
        return std::find(children.begin(), children.end(), child) != children.end();
    }

    void swap_child(Window* old_child, Window* new_child) {
        for (size_t i = 0; i < children.size(); i++) {
            if (children[i] == old_child) {
                old_child->parent = nullptr;
                children[i] = new_child;
                new_child->parent = this;
                return;
            }
        }
    }

    void add_child(Window* child) {
        children.push_back(child);
        child->parent = this;
    }

    void remove_child(Window* child) {
        children.erase(std::remove(children.begin(), children.end(), child), children.end());
        child->parent = nullptr;
    }

    void arrange_children() {
        resize_strategy->execute(children, bounds, bounds);
    }

    ContainerWindow* get_container() override { return this; }

    void draw() override {
        for (auto child : children) {
            child->draw();
        }
    }

    void show() override {
        for (auto child : children) {
            child->show();
        }
    }

    bool resize(Dimension new_bounds) override {
        Window::resize(bounds);

        if (resize_strategy->execute(children, bounds, bounds)) {
            this->bounds = new_bounds;
            return true;
        }

        return false;
    }
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
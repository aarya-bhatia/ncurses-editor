#pragma once
#include "Window.h"
#include <vector>

struct WindowResizeStrategy
{
    virtual ~WindowResizeStrategy() = default;
    virtual bool execute(std::vector<Window*>& children, Dimension prev_bound, Dimension new_bound) = 0;
};

struct ProportionalResizeStrategy : public WindowResizeStrategy
{
    bool execute(std::vector<Window*>& children, Dimension prev_bound, Dimension new_bound)
    {
        if (prev_bound.height == 0 || prev_bound.width == 0) {
            return false;
        }

        for (Window* child : children) {
            Dimension child_bound = new_bound;
            child_bound.height = child->bounds.height * new_bound.height / prev_bound.height;
            child_bound.width = child->bounds.width * new_bound.width / prev_bound.width;
            child->resize(child_bound);
        }

        return true;
    }

};

struct HSplitResizeStrategy : public WindowResizeStrategy
{
    bool execute(std::vector<Window*>& children, Dimension prev_bound, Dimension new_bound)
    {
        if (children.empty())
        {
            return false;
        }

        unsigned child_height = new_bound.height / children.size();

        for (int i = 0; i < children.size(); i++) {
            children[i]->resize(Dimension(new_bound.x, new_bound.y + i * child_height, new_bound.width, child_height));
        }

        return true;
    }
};

struct VSplitResizeStrategy : public WindowResizeStrategy
{
    bool execute(std::vector<Window*>& children, Dimension prev_bound, Dimension new_bound)
    {
        if (children.empty())
        {
            return false;
        }

        unsigned child_width = new_bound.width / children.size();

        for (int i = 0; i < children.size(); i++) {
            children[i]->resize(Dimension(new_bound.x + i * child_width, new_bound.y, child_width, new_bound.height));
        }

        return true;
    }
};
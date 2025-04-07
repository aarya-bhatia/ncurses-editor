#pragma once
#include "Window.h"
#include "log.h"
#include <vector>

struct WindowResizeStrategy
{
    virtual ~WindowResizeStrategy() = default;
    virtual bool execute(std::vector<Window*>& children, Dimension prev_bound, Dimension new_bound) = 0;
    virtual bool is_executable(std::vector<Window*>& children, Dimension prev_bound, Dimension new_bound) = 0;
};

struct ProportionalResizeStrategy : public WindowResizeStrategy
{
    bool execute(std::vector<Window*>& children, Dimension prev_bound, Dimension new_bound) override
    {
        log_debug("Resizing child nodes to %d lines x %d cols", new_bound.height, new_bound.width);

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

    bool is_executable(std::vector<Window*>& children, Dimension prev_bound, Dimension new_bound) override
    {
        if (prev_bound.height == 0 || prev_bound.width == 0) {
            return false;
        }

        for (Window* child : children) {
            Dimension child_bound = new_bound;
            child_bound.height = child->bounds.height * new_bound.height / prev_bound.height;
            child_bound.width = child->bounds.width * new_bound.width / prev_bound.width;
            if (!child->resizable(child_bound)) {
                return false;
            }
        }

        return true;
    }

};

struct HSplitResizeStrategy : public WindowResizeStrategy
{
    bool execute(std::vector<Window*>& children, Dimension prev_bound, Dimension new_bound) override;
    bool is_executable(std::vector<Window*>& children, Dimension prev_bound, Dimension new_bound) override;
};

struct VSplitResizeStrategy : public WindowResizeStrategy
{
    bool execute(std::vector<Window*>& children, Dimension prev_bound, Dimension new_bound) override;
    bool is_executable(std::vector<Window*>& children, Dimension prev_bound, Dimension new_bound) override;
};
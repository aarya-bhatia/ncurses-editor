#include "WindowResizeStrategies.h"
#include "log.h"

bool HSplitResizeStrategy::execute(std::vector<Window*>& children, Dimension prev_bound, Dimension new_bound)
{
    log_debug("Resizing child nodes to %d lines x %d cols", new_bound.height, new_bound.width);

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

bool HSplitResizeStrategy::is_executable(std::vector<Window*>& children, Dimension prev_bound, Dimension new_bound)
{
    if (children.empty())
    {
        return false;
    }

    unsigned child_height = new_bound.height / children.size();

    for (int i = 0; i < children.size(); i++) {
        if (!children[i]->resizable(Dimension(new_bound.x, new_bound.y + i * child_height, new_bound.width, child_height))) {
            return false;
        }
    }

    return true;

}
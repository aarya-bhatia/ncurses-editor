#include "WindowResizeStrategies.h"

bool VSplitResizeStrategy::execute(std::vector<Window*>& children, Dimension prev_bound, Dimension new_bound)
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

bool VSplitResizeStrategy::is_executable(std::vector<Window*>& children, Dimension prev_bound, Dimension new_bound)
{
    if (children.empty())
    {
        return false;
    }

    unsigned child_width = new_bound.width / children.size();

    for (int i = 0; i < children.size(); i++) {
        if (!children[i]->resizable(Dimension(new_bound.x + i * child_width, new_bound.y, child_width, new_bound.height))) {
            return false;
        }
    }

    return true;

}
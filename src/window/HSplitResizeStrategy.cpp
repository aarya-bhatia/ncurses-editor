#include "WindowResizeStrategies.h"

bool HSplitResizeStrategy::execute(std::vector<Window*>& children, Dimension prev_bound, Dimension new_bound)
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
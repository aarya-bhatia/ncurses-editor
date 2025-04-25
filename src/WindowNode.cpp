#include "WindowNode.h"
#include <assert.h>

template<typename T>
void WindowNode<T>::splith()
{
    if (!splith_allowed()) { return; }

    layout = HSPLIT;

    Dimension d1(bounds.x, bounds.y, bounds.width, bounds.height / 2);
    Dimension d2(bounds.x, bounds.y + bounds.height / 2, bounds.width, bounds.height / 2);

    WindowNode* child1 = new WindowNode(d1, this);
    WindowNode* child2 = new WindowNode(d2, this);

    children.push_back(child1);
    children.push_back(child2);

    child1->content = this->content;
    child1->resize(child1->bounds);
    this->content = nullptr;

    log_info("horizontal split complete");

    assert(child1 == get_top_child());
    assert(child2 == get_bottom_child());
    assert(child1->find_bottom_adjacent_node() == child2);
    assert(child2->find_top_adjacent_node() == child1);
    assert(find_top_content_node() == child1);
    assert(find_bottom_content_node() == child2);
}

template<typename T>
void WindowNode<T>::splitv()
{
    if (!splitv_allowed()) { return; }

    layout = VSPLIT;

    Dimension d1(bounds.x, bounds.y, bounds.width / 2, bounds.height);
    Dimension d2(bounds.x + bounds.width / 2, bounds.y, bounds.width / 2, bounds.height);

    WindowNode* child1 = new WindowNode(d1, this);
    WindowNode* child2 = new WindowNode(d2, this);

    children.push_back(child1);
    children.push_back(child2);

    child1->content = this->content;
    child1->resize(child1->bounds);
    this->content = nullptr;

    log_info("horizontal split complete");

    assert(child1 == get_left_child());
    assert(child2 == get_right_child());
    assert(child1->find_right_adjacent_node() == child2);
    assert(child2->find_left_adjacent_node() == child1);
    assert(find_left_content_node() == child1);
    assert(find_right_content_node() == child2);
}
#include "WMNode.h"
#include <assert.h>

void WMNode::resize(Dimension d)
{
    for (auto* child : children)
    {
        Dimension child_d = d;
        child_d.width *= d.width / bounds.width;
        child_d.height *= d.height / bounds.height;
        child->resize(child_d);
    }

    tabs.resize(d);
    bounds = d;
}

void WMNode::splith()
{
    if (!splith_allowed()) { return; }

    layout = HSPLIT;

    Dimension d1(bounds.x, bounds.y, bounds.width, bounds.height / 2);
    Dimension d2(bounds.x, bounds.y + bounds.height / 2, bounds.width, bounds.height / 2);

    WMNode* child1 = new WMNode(d1, this);
    WMNode* child2 = new WMNode(d2, this);

    children.push_back(child1);
    children.push_back(child2);

    if (!tabs.empty()) {
        child1->open_tab(tabs.get_file());
        child2->open_tab(tabs.get_file());
        tabs.clear();
    }

    log_info("horizontal split complete");

    assert(child1 == get_top_child());
    assert(child2 == get_bottom_child());
    assert(child1->find_bottom_adjacent_node() == child2);
    assert(child2->find_top_adjacent_node() == child1);
    assert(find_top_content_node() == child1);
    assert(find_bottom_content_node() == child2);
}

void WMNode::splitv()
{
    if (!splitv_allowed()) { return; }

    layout = VSPLIT;

    Dimension d1(bounds.x, bounds.y, bounds.width / 2, bounds.height);
    Dimension d2(bounds.x + bounds.width / 2, bounds.y, bounds.width / 2, bounds.height);

    WMNode* child1 = new WMNode(d1, this);
    WMNode* child2 = new WMNode(d2, this);

    children.push_back(child1);
    children.push_back(child2);

    if (!tabs.empty()) {
        child1->open_tab(tabs.get_file());
        child2->open_tab(tabs.get_file());
        tabs.clear();
    }

    log_info("horizontal split complete");

    assert(child1 == get_left_child());
    assert(child2 == get_right_child());
    assert(child1->find_right_adjacent_node() == child2);
    assert(child2->find_left_adjacent_node() == child1);
    assert(find_left_content_node() == child1);
    assert(find_right_content_node() == child2);
}
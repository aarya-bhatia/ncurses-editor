#include "WMNode.h"
#include <assert.h>

void WMNode::close_tab()
{
    if (current_tab == tabs.end()) {
        log_debug("no tab is open");
        return;
    }

    Tab new_tab = tabs.end();
    if (tabs.size() > 1) {
        if (std::next(current_tab) != tabs.end()) {
            new_tab = std::next(current_tab);
        }
        else {
            new_tab = std::prev(current_tab);
        }
    }

    log_info("tab closed");
    (*current_tab)->unfocus();
    (*current_tab)->clear();
    (*current_tab)->show();
    delete (*current_tab);
    tabs.erase(current_tab);

    current_tab = tabs.end();
    open_tab(new_tab);
}

void WMNode::resize(Dimension d)
{
    for (auto* child : children)
    {
        Dimension child_d = d;
        child_d.width *= d.width / bounds.width;
        child_d.height *= d.height / bounds.height;
        child->resize(child_d);
    }

    // note: resize other tabs when they are opened.
    if (current_tab != tabs.end()) {
        (*current_tab)->resize(d);
    }

    bounds = d;
}

Window* WMNode::open_tab(Tab tab)
{
    if (current_tab != tabs.end()) {
        (*current_tab)->unfocus();
    }

    current_tab = tab;
    (*current_tab)->resize(bounds);

    if (focused) {
        (*current_tab)->focus();
    }

    log_info("new tab opened in WMNode %s", bounds.debug_string().c_str());
    return *current_tab;
}

void WMNode::splith()
{
    if (!split_allowed()) { return; }

    layout = HSPLIT;

    Dimension d1(bounds.x, bounds.y, bounds.width, bounds.height / 2);
    Dimension d2(bounds.x, bounds.y + bounds.height / 2, bounds.width, bounds.height / 2);

    WMNode* child1 = new WMNode(d1, this);
    WMNode* child2 = new WMNode(d2, this);

    children.push_back(child1);
    children.push_back(child2);

    if (current_tab != tabs.end()) {
        child1->open_tab((*current_tab)->get_file());
        child2->open_tab((*current_tab)->get_file());
        close_tab();
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
    if (!split_allowed()) { return; }

    layout = VSPLIT;

    Dimension d1(bounds.x, bounds.y, bounds.width / 2, bounds.height);
    Dimension d2(bounds.x + bounds.width / 2, bounds.y, bounds.width / 2, bounds.height);

    WMNode* child1 = new WMNode(d1, this);
    WMNode* child2 = new WMNode(d2, this);

    children.push_back(child1);
    children.push_back(child2);

    if (current_tab != tabs.end()) {
        child1->open_tab((*current_tab)->get_file());
        child2->open_tab((*current_tab)->get_file());
        close_tab();
    }

    log_info("horizontal split complete");

    assert(child1 == get_left_child());
    assert(child2 == get_right_child());
    assert(child1->find_right_adjacent_node() == child2);
    assert(child2->find_left_adjacent_node() == child1);
    assert(find_left_content_node() == child1);
    assert(find_right_content_node() == child2);
}
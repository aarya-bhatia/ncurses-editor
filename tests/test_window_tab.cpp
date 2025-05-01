#include "catch.hpp"
#include "WindowTab.h"
#include "TestWindow.h"

#define MY_TEST_SUITE "[window_tab]"

TEST_CASE("WindowTab initialization", MY_TEST_SUITE) {
    Dimension bounds(0, 0, 100, 100);
    WindowTab tab(bounds);
    
    REQUIRE(tab.root_node != nullptr);
    REQUIRE(tab.focused_node != nullptr);
    REQUIRE(tab.focused_node == tab.root_node);
    REQUIRE(tab.bounds == bounds);
}

TEST_CASE("WindowTab set focused node content", MY_TEST_SUITE) {
    Dimension bounds(0, 0, 100, 100);
    WindowTab tab(bounds);
    TestWindow* window = new TestWindow();
    
    tab.set_focused_node_content(window);
    REQUIRE(tab.get_focused_node_content() == window);
    REQUIRE(tab.focused_node->content == window);
}

TEST_CASE("WindowTab horizontal split", MY_TEST_SUITE) {
    Dimension bounds(0, 0, 100, 100);
    WindowTab tab(bounds);
    TestWindow* window = new TestWindow();
    
    tab.set_focused_node_content(window);
    bool result = tab.splith();
    
    REQUIRE(result == true);
    REQUIRE(tab.root_node->layout == WindowNode::Layout::HSPLIT);
    REQUIRE(tab.root_node->children.size() == 2);
    REQUIRE(tab.focused_node == tab.root_node->get_top_child());
    REQUIRE(tab.focused_node->content == window);
    
    // Check the dimensions of the split
    REQUIRE(tab.root_node->get_top_child()->bounds == Dimension(0, 0, 100, 50));
    REQUIRE(tab.root_node->get_bottom_child()->bounds == Dimension(0, 50, 100, 50));
}

TEST_CASE("WindowTab vertical split", MY_TEST_SUITE) {
    Dimension bounds(0, 0, 100, 100);
    WindowTab tab(bounds);
    TestWindow* window = new TestWindow();
    
    tab.set_focused_node_content(window);
    bool result = tab.splitv();
    
    REQUIRE(result == true);
    REQUIRE(tab.root_node->layout == WindowNode::Layout::VSPLIT);
    REQUIRE(tab.root_node->children.size() == 2);
    REQUIRE(tab.focused_node == tab.root_node->get_left_child());
    REQUIRE(tab.focused_node->content == window);
    
    // Check the dimensions of the split
    REQUIRE(tab.root_node->get_left_child()->bounds == Dimension(0, 0, 50, 100));
    REQUIRE(tab.root_node->get_right_child()->bounds == Dimension(50, 0, 50, 100));
}

TEST_CASE("WindowTab focus navigation", MY_TEST_SUITE) {
    Dimension bounds(0, 0, 100, 100);
    WindowTab tab(bounds);
    
    // Create a 2x2 grid of windows
    tab.splith();
    tab.splitv();
    tab.focus_bottom();
    tab.splitv();
    
    // We should now have a 2x2 grid with the focus on the bottom-left window
    WindowNode* top_left = tab.root_node->get_top_child()->get_left_child();
    // WindowNode* top_right = tab.root_node->get_top_child()->get_right_child();
    WindowNode* bottom_left = tab.root_node->get_bottom_child()->get_left_child();
    WindowNode* bottom_right = tab.root_node->get_bottom_child()->get_right_child();
    
    REQUIRE(tab.focused_node == bottom_left);
    
    // Test focus_right
    bool result = tab.focus_right();
    REQUIRE(result == true);
    REQUIRE(tab.focused_node == bottom_right);
    
    // Test focus_left
    result = tab.focus_left();
    REQUIRE(result == true);
    REQUIRE(tab.focused_node == bottom_left);
    
    // Test focus_top
    result = tab.focus_top();
    REQUIRE(result == true);
    REQUIRE(tab.focused_node == top_left);
    
    // Test focus_bottom
    result = tab.focus_bottom();
    REQUIRE(result == true);
    REQUIRE(tab.focused_node == bottom_left);
    
    // Test edge cases (should return false when no node in that direction)
    tab.focus_top(); // Now at top_left
    result = tab.focus_top();
    REQUIRE(result == false);
    REQUIRE(tab.focused_node == top_left);
    
    result = tab.focus_left();
    REQUIRE(result == false);
    REQUIRE(tab.focused_node == top_left);
}

TEST_CASE("WindowTab resize", MY_TEST_SUITE) {
    Dimension original_bounds(0, 0, 100, 100);
    WindowTab tab(original_bounds);
    
    Dimension new_bounds(0, 0, 200, 150);
    tab.resize(new_bounds);
    
    REQUIRE(tab.bounds == new_bounds);
    REQUIRE(tab.root_node->bounds == new_bounds);
}

TEST_CASE("WindowTab split not allowed", MY_TEST_SUITE) {
    // Test with dimensions too small for splitting
    Dimension small_bounds(0, 0, 3, 3);
    WindowTab tab(small_bounds);
    
    bool h_result = tab.splith();
    REQUIRE(h_result == false);
    
    bool v_result = tab.splitv();
    REQUIRE(v_result == false);
    
    // Verify the structure hasn't changed
    REQUIRE(tab.root_node->children.size() == 0);
    REQUIRE(tab.root_node->layout == WindowNode::Layout::NORMAL);
}

TEST_CASE("WindowTab set_focused_node", MY_TEST_SUITE) {
    Dimension bounds(0, 0, 100, 100);
    WindowTab tab(bounds);
    
    tab.splith();
    WindowNode* top_node = tab.root_node->get_top_child();
    WindowNode* bottom_node = tab.root_node->get_bottom_child();
    
    // Focus should be on top node after split
    REQUIRE(tab.focused_node == top_node);
    
    // Change focus to bottom node
    tab.set_focused_node(bottom_node);
    REQUIRE(tab.focused_node == bottom_node);
}

TEST_CASE("WindowTab with content in multiple nodes", MY_TEST_SUITE) {
    Dimension bounds(0, 0, 100, 100);
    WindowTab tab(bounds);
    
    TestWindow* window1 = new TestWindow();
    tab.set_focused_node_content(window1);
    
    tab.splith();
    
    TestWindow* window2 = new TestWindow();
    tab.focus_bottom();
    tab.set_focused_node_content(window2);
    
    // Top node should have window1, bottom node should have window2
    REQUIRE(tab.root_node->get_top_child()->content == window1);
    REQUIRE(tab.root_node->get_bottom_child()->content == window2);
    
    // Current focus should be on bottom node with window2
    REQUIRE(tab.focused_node->content == window2);
    REQUIRE(tab.get_focused_node_content() == window2);
    
    // Change focus back to top
    tab.focus_top();
    REQUIRE(tab.get_focused_node_content() == window1);
}
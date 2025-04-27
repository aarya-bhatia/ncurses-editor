#include "catch.hpp"
#include "WindowManager.h"
#include "WindowNode.h"
#include "Window.h"

#include "TestWindow.h"

#define MY_TEST_SUITE "[window_node]"

TEST_CASE("check horizontal split", MY_TEST_SUITE) {
    Dimension orig_bounds = Dimension(50, 50, 200, 200);
    WindowNode<Window*>* node = new WindowNode<Window*>(orig_bounds);

    assert(node->splith_allowed());
    node->splith();

    assert(node->children.size() == 2);

    Dimension bounds = node->children[0]->bounds;
    assert(bounds.x == 50);
    assert(bounds.y == 50);
    assert(bounds.width == 200);
    assert(bounds.height == 100);

    bounds = node->children[1]->bounds;
    assert(bounds.x == 50);
    assert(bounds.y == 150);
    assert(bounds.width == 200);
    assert(bounds.height == 100);

    delete node;
}

TEST_CASE("check vertical split", MY_TEST_SUITE) {
    Dimension orig_bounds = Dimension(50, 50, 200, 200);
    WindowNode<Window*>* node = new WindowNode<Window*>(orig_bounds);

    assert(node->splitv_allowed());
    node->splitv();

    assert(node->children.size() == 2);

    Dimension bounds = node->children[0]->bounds;
    assert(bounds.x == 50);
    assert(bounds.y == 50);
    assert(bounds.width == 100);
    assert(bounds.height == 200);

    bounds = node->children[1]->bounds;
    assert(bounds.x == 150);
    assert(bounds.y == 50);
    assert(bounds.width == 100);
    assert(bounds.height == 200);

    delete node;
}


TEST_CASE("check content is transferred to first child when split", MY_TEST_SUITE) {
    WindowNode<TestWindow*>* node = new WindowNode<TestWindow*>(Dimension(0, 0, 100, 100));
    TestWindow* content = new TestWindow();
    node->content = content;
    node->splith();
    REQUIRE(node->children[0]->content == content);
    REQUIRE(node->children[1]->content == nullptr);
    REQUIRE(node->content == nullptr);

    delete node;
}

TEST_CASE("check split allowed works", MY_TEST_SUITE) {
    WindowNode<TestWindow*> node(Dimension(0, 0, 6, 6));
    REQUIRE(node.splith_allowed());
    REQUIRE(node.splitv_allowed());

    node = WindowNode<TestWindow*>(Dimension(0, 0, 3, 3));
    REQUIRE(!node.splith_allowed());
    REQUIRE(!node.splitv_allowed());

    node = WindowNode<TestWindow*>(Dimension(0, 0, 6, 3));
    REQUIRE(!node.splith_allowed());
    REQUIRE(node.splitv_allowed());

    node = WindowNode<TestWindow*>(Dimension(0, 0, 3, 6));
    REQUIRE(node.splith_allowed());
    REQUIRE(!node.splitv_allowed());
}

TEST_CASE("multiple splits", MY_TEST_SUITE) {
    WindowNode<TestWindow*> root(Dimension(0, 0, 100, 100));
    root.splith();
    auto top_node = root.children[0];
    auto bottom_node = root.children[1];
    top_node->splitv();
    bottom_node->splitv();

    REQUIRE(root.count_nodes() == 7);

    auto top_left = top_node->children[0];
    auto top_right = top_node->children[1];
    auto bottom_left = bottom_node->children[0];
    auto bottom_right = bottom_node->children[1];

    REQUIRE(root.children.size() == 2);
    REQUIRE(top_node->children.size() == 2);
    REQUIRE(bottom_node->children.size() == 2);
    REQUIRE(top_node->children[0]->children.size() == 0);
    REQUIRE(top_node->children[1]->children.size() == 0);
    REQUIRE(bottom_node->children[0]->children.size() == 0);
    REQUIRE(bottom_node->children[1]->children.size() == 0);

    REQUIRE(top_left->bounds == Dimension(0, 0, 50, 50));
    REQUIRE(top_right->bounds == Dimension(50, 0, 50, 50));
    REQUIRE(bottom_left->bounds == Dimension(0, 50, 50, 50));
    REQUIRE(bottom_right->bounds == Dimension(50, 50, 50, 50));

    REQUIRE(root.bounds.center() == Point(50, 50));
    REQUIRE(top_left->bounds.center() == Point(25, 25));
    REQUIRE(top_right->bounds.center() == Point(75, 25));
    REQUIRE(bottom_left->bounds.center() == Point(25, 75));
    REQUIRE(bottom_right->bounds.center() == Point(75, 75));

    REQUIRE(top_node->find_nearest_child(top_left) == top_left);
    REQUIRE(top_node->find_nearest_child(top_right) == top_right);
    REQUIRE(top_node->find_nearest_child(bottom_left) == top_left);
    REQUIRE(top_node->find_nearest_child(bottom_right) == top_right);

    REQUIRE(bottom_node->find_nearest_child(top_left) == bottom_left);
    REQUIRE(bottom_node->find_nearest_child(top_right) == bottom_right);
    REQUIRE(bottom_node->find_nearest_child(bottom_left) == bottom_left);
    REQUIRE(bottom_node->find_nearest_child(bottom_right) == bottom_right);

    REQUIRE(top_node->find_bottom_content_node(bottom_left) == top_left);
    REQUIRE(top_node->find_bottom_content_node(bottom_right) == top_right);
    REQUIRE(bottom_node->find_top_content_node(top_left) == bottom_left);
    REQUIRE(bottom_node->find_top_content_node(top_right) == bottom_right);

    REQUIRE(top_node->find_left_content_node(nullptr) == top_left);
    REQUIRE(top_node->find_right_content_node(nullptr) == top_right);
    REQUIRE(bottom_node->find_left_content_node(nullptr) == bottom_left);
    REQUIRE(bottom_node->find_right_content_node(nullptr) == bottom_right);

    REQUIRE(top_left->find_right_adjacent_node() == top_right);
    REQUIRE(top_left->find_left_adjacent_node() == nullptr);
    REQUIRE(top_left->find_bottom_adjacent_node() == bottom_left);
    REQUIRE(top_left->find_top_adjacent_node() == nullptr);

    REQUIRE(bottom_left->find_right_adjacent_node() == bottom_right);
    REQUIRE(bottom_left->find_left_adjacent_node() == nullptr);
    REQUIRE(bottom_left->find_bottom_adjacent_node() == nullptr);
    REQUIRE(bottom_left->find_top_adjacent_node() == top_left);

    REQUIRE(top_right->find_right_adjacent_node() == nullptr);
    REQUIRE(top_right->find_left_adjacent_node() == top_left);
    REQUIRE(top_right->find_bottom_adjacent_node() == bottom_right);
    REQUIRE(top_right->find_top_adjacent_node() == nullptr);

    REQUIRE(bottom_right->find_right_adjacent_node() == nullptr);
    REQUIRE(bottom_right->find_left_adjacent_node() == bottom_left);
    REQUIRE(bottom_right->find_bottom_adjacent_node() == nullptr);
    REQUIRE(bottom_right->find_top_adjacent_node() == top_right);
}

TEST_CASE("check focus is applied properly", MY_TEST_SUITE) {
}

TEST_CASE("check resize works properly", MY_TEST_SUITE) {

}

TEST_CASE("check navigation in node tree works", MY_TEST_SUITE) {

}
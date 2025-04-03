#include "catch.hpp"
#include "window/WindowManager.h"
#include "window/Window.h"
#include "window/ContentWindow.h"
#include "TestContentView.h"

TEST_CASE("WM works with one content window", "[windows]") {
    Dimension bounds(0, 0, 100, 100);
    WindowManager wm(bounds);

    wm.set_content(new TestContentView());

    ContentWindow* current_window = wm.get_content_node();
    REQUIRE(current_window);
    REQUIRE(current_window->get_bounds() == bounds);
    REQUIRE(current_window->get_content() == current_window);
    REQUIRE(current_window->get_container() == nullptr);
    REQUIRE(current_window->parent == nullptr);

    TestContentView* view = dynamic_cast<TestContentView*>(current_window);
    REQUIRE(view);
    REQUIRE(!view->draw_called);
    REQUIRE(!view->show_called);
    REQUIRE(view->resize_called);

    view->reset();

    wm.draw();
    REQUIRE(view->draw_called);
    REQUIRE(!view->show_called);
    REQUIRE(!view->resize_called);

    wm.show();
    REQUIRE(view->show_called);
    REQUIRE(!view->resize_called);
}

TEST_CASE("check content window is resized when added to WM", "[windows]") {
    Dimension bounds(0, 0, 100, 100);
    WindowManager wm(bounds);

    TestContentView* view = new TestContentView(Dimension(0, 0, 50, 50));
    wm.set_content(view);

    ContentWindow* current_window = wm.get_content_node();
    REQUIRE(current_window);
    REQUIRE(current_window->get_bounds() == bounds);
}

TEST_CASE("check window container adds and resizes children properly", "[windows]") {
    Dimension bounds(0, 0, 100, 100);

    HSplitContainerWindow* container = new HSplitContainerWindow(bounds);
    container->add_child(new TestContentView(bounds));
    container->add_child(new TestContentView(bounds));
    container->arrange_children();

    REQUIRE(dynamic_cast<HSplitResizeStrategy*>(container->resize_strategy) != nullptr);

    REQUIRE(container->count_children() == 2);
    REQUIRE(container->get_container() == container);
    REQUIRE(container->get_content() == nullptr);
    REQUIRE(container->get_bounds() == bounds);
    REQUIRE(container->parent == nullptr);
    REQUIRE(container->children[0]->parent == container);
    REQUIRE(container->children[0]->get_bounds() == Dimension(0, 0, 100, 50));
    REQUIRE(container->children[1]->parent == container);
    REQUIRE(container->children[1]->get_bounds() == Dimension(0, 50, 100, 50));
}

TEST_CASE("check single split works", "[windows]"){
    Dimension bounds(0, 0, 100, 100);
    WindowManager wm(bounds);
    wm.set_content(new TestContentView(bounds));
    wm.split_horizontal(new TestContentView(bounds));

    ContentWindow *node = wm.get_content_node();
    REQUIRE(node);
    REQUIRE(node != wm.root_node);
    REQUIRE(node->parent != nullptr);
    REQUIRE(node->parent->count_children() == 2);

    wm.draw();

    ContentWindow *content = node->parent->children[0]->get_content();
    TestContentView *view = dynamic_cast<TestContentView*>(content);
    REQUIRE(view->draw_called);

    content = node->parent->children[1]->get_content();
    view = dynamic_cast<TestContentView*>(content);
    REQUIRE(view->draw_called);
}

TEST_CASE("Check multiple splits", "[windows]") {
    Dimension bounds(0, 0, 100, 100);
    WindowManager wm(bounds);
    wm.set_content(new TestContentView(bounds));
    wm.split_horizontal(new TestContentView(bounds));
    wm.split_vertical(new TestContentView(bounds));

    ContentWindow *node = wm.get_content_node();
    REQUIRE(node);
    REQUIRE(node != wm.root_node);
    REQUIRE(node->parent != nullptr);
    REQUIRE(node->parent->count_children() == 2);
    REQUIRE(node->get_bounds() == Dimension(0, 0, 50, 50));

    REQUIRE(node->parent->children[0] == node);

    ContentWindow *sibling = node->parent->children[1]->get_content();
    REQUIRE(sibling);
    REQUIRE(sibling->get_bounds() == Dimension(50, 0, 50, 50));

    REQUIRE(node->parent->parent != nullptr);

    REQUIRE(node->parent->get_bounds() == Dimension(0, 0, 100, 50));
    REQUIRE(node->parent->parent->get_bounds() == Dimension(0, 0, 100, 100));

    REQUIRE(node->parent->count_children() == 2);
    REQUIRE(node->parent->parent->count_children() == 2);
    REQUIRE(node->parent->parent->children[0] == node->parent);
    REQUIRE(node->parent->parent->children[1]);
    REQUIRE(node->parent->parent->children[1]->get_bounds() == Dimension(0, 50, 100, 50));
}
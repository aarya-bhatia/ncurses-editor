#include "catch.hpp"
#include "WindowManager.h"
#include "Window.h"


// TEST_CASE("check horizontal split resize strategy", "[windows]") {
//     HSplitResizeStrategy* resize_strategy = new HSplitResizeStrategy;
//     std::vector<Window*> children;
//     Dimension orig_bounds = Dimension(50, 50, 200, 200);
//     children.push_back(new TestContentView(Dimension(orig_bounds)));
//     children.push_back(new TestContentView(Dimension(orig_bounds)));
//     assert(resize_strategy->execute(children, orig_bounds, orig_bounds) == true);
//     assert(children.size() == 2);
//     assert(children[0]);
//     assert(children[1]);

//     Dimension bounds = children[0]->get_bounds();
//     assert(bounds.x == 50);
//     assert(bounds.y == 50);
//     assert(bounds.width == 200);
//     assert(bounds.height == 100);

//     bounds = children[1]->get_bounds();
//     assert(bounds.x == 50);
//     assert(bounds.y == 150);
//     assert(bounds.width == 200);
//     assert(bounds.height == 100);

//     delete resize_strategy;
//     delete children[0];
//     delete children[1];
// }

// TEST_CASE("check vertical split resize strategy", "[windows]") {
//     VSplitResizeStrategy* resize_strategy = new VSplitResizeStrategy;
//     std::vector<Window*> children;
//     Dimension orig_bounds = Dimension(50, 50, 200, 200);
//     children.push_back(new TestContentView(Dimension(orig_bounds)));
//     children.push_back(new TestContentView(Dimension(orig_bounds)));
//     assert(resize_strategy->execute(children, orig_bounds, orig_bounds) == true);
//     assert(children.size() == 2);
//     assert(children[0]);
//     assert(children[1]);

//     Dimension bounds = children[0]->get_bounds();
//     assert(bounds.x == 50);
//     assert(bounds.y == 50);
//     assert(bounds.width == 100);
//     assert(bounds.height == 200);

//     bounds = children[1]->get_bounds();
//     assert(bounds.x == 150);
//     assert(bounds.y == 50);
//     assert(bounds.width == 100);
//     assert(bounds.height == 200);

//     delete resize_strategy;
//     delete children[0];
//     delete children[1];
// }

// TEST_CASE("check container window arrange children", "[windows]") {
//     Dimension bounds(0, 0, 200, 200);

//     HSplitContainerWindow* container1 = new HSplitContainerWindow(bounds);
//     container1->add_child(new TestContentView());
//     container1->add_child(new HSplitContainerWindow(bounds));
//     ContainerWindow* container2 = container1->children[1]->get_container();
//     container2->add_child(new TestContentView());
//     container2->add_child(new TestContentView());

//     container1->arrange_children();

//     assert(container1->count_children() == 2);
//     assert(container1->get_bounds() == Dimension(0, 0, 200, 200));
//     assert(container1->children[0]);
//     assert(container1->children[0]->get_content());
//     assert(container1->children[0]->get_bounds() == Dimension(0, 0, 200, 100));

//     assert(container2->count_children() == 2);
//     assert(container2->get_bounds() == Dimension(0, 100, 200, 100));
//     assert(container2->children[0]);
//     assert(container2->children[0]->get_content());
//     assert(container2->children[0]->get_bounds() == Dimension(0, 100, 200, 50));

//     assert(container2->children[1]);
//     assert(container2->children[1]->get_content());
//     assert(container2->children[1]->get_bounds() == Dimension(0, 150, 200, 50));

//     delete container1;
// }

// TEST_CASE("WM works with one content window", "[windows]") {
//     Dimension bounds(0, 0, 100, 100);
//     WindowManager wm(bounds);

//     wm.set_content(new TestContentView());

//     ContentWindow* current_window = wm.get_content_node();
//     REQUIRE(current_window);
//     REQUIRE(current_window->get_bounds() == bounds);
//     REQUIRE(current_window->get_content() == current_window);
//     REQUIRE(current_window->get_container() == nullptr);
//     REQUIRE(current_window->parent == nullptr);

//     TestContentView* view = dynamic_cast<TestContentView*>(current_window);
//     REQUIRE(view);
//     REQUIRE(!view->draw_called);
//     REQUIRE(!view->show_called);
//     REQUIRE(view->resize_called);

//     view->reset();

//     wm.draw();
//     REQUIRE(view->draw_called);
//     REQUIRE(!view->show_called);
//     REQUIRE(!view->resize_called);

//     wm.show();
//     REQUIRE(view->show_called);
//     REQUIRE(!view->resize_called);
// }

// TEST_CASE("check content window is resized when added to WM", "[windows]") {
//     Dimension bounds(0, 0, 100, 100);
//     WindowManager wm(bounds);

//     TestContentView* view = new TestContentView(Dimension(0, 0, 50, 50));
//     wm.set_content(view);

//     ContentWindow* current_window = wm.get_content_node();
//     REQUIRE(current_window);
//     REQUIRE(current_window->get_bounds() == bounds);
// }

// TEST_CASE("check window container adds and resizes children properly", "[windows]") {
//     Dimension bounds(0, 0, 100, 100);

//     HSplitContainerWindow* container = new HSplitContainerWindow(bounds);
//     container->add_child(new TestContentView(bounds));
//     container->add_child(new TestContentView(bounds));
//     container->arrange_children();

//     REQUIRE(dynamic_cast<HSplitResizeStrategy*>(container->resize_strategy) != nullptr);

//     REQUIRE(container->count_children() == 2);
//     REQUIRE(container->get_container() == container);
//     REQUIRE(container->get_content() == nullptr);
//     REQUIRE(container->get_bounds() == bounds);
//     REQUIRE(container->parent == nullptr);
//     REQUIRE(container->children[0]->parent == container);
//     REQUIRE(container->children[0]->get_bounds() == Dimension(0, 0, 100, 50));
//     REQUIRE(container->children[1]->parent == container);
//     REQUIRE(container->children[1]->get_bounds() == Dimension(0, 50, 100, 50));
// }

// TEST_CASE("check single split works", "[windows]") {
//     Dimension bounds(0, 0, 100, 100);
//     WindowManager wm(bounds);
//     wm.set_content(new TestContentView(bounds));
//     wm.split_horizontal(new TestContentView(bounds));

//     ContentWindow* node = wm.get_content_node();
//     REQUIRE(node);
//     REQUIRE(node != wm.root_node);
//     REQUIRE(node->parent != nullptr);
//     REQUIRE(node->parent->count_children() == 2);

//     wm.draw();

//     ContentWindow* content = node->parent->children[0]->get_content();
//     TestContentView* view = dynamic_cast<TestContentView*>(content);
//     REQUIRE(view->draw_called);

//     content = node->parent->children[1]->get_content();
//     view = dynamic_cast<TestContentView*>(content);
//     REQUIRE(view->draw_called);
// }

// TEST_CASE("Check multiple splits", "[windows]") {
//     Dimension bounds(0, 0, 100, 100);
//     WindowManager wm(bounds);
//     wm.set_content(new TestContentView(bounds));
//     wm.split_horizontal(new TestContentView(bounds));
//     wm.split_vertical(new TestContentView(bounds));

//     ContentWindow* node = wm.get_content_node();
//     REQUIRE(node);
//     REQUIRE(node != wm.root_node);
//     REQUIRE(node->parent != nullptr);
//     REQUIRE(node->parent->count_children() == 2);
//     REQUIRE(node->get_bounds() == Dimension(0, 0, 50, 50));

//     REQUIRE(node->parent->children[0] == node);

//     ContentWindow* sibling = node->parent->children[1]->get_content();
//     REQUIRE(sibling);
//     REQUIRE(sibling->get_bounds() == Dimension(50, 0, 50, 50));

//     REQUIRE(node->parent->parent != nullptr);

//     REQUIRE(node->parent->get_bounds() == Dimension(0, 0, 100, 50));
//     REQUIRE(node->parent->parent->get_bounds() == Dimension(0, 0, 100, 100));

//     REQUIRE(node->parent->count_children() == 2);
//     REQUIRE(node->parent->parent->count_children() == 2);
//     REQUIRE(node->parent->parent->children[0] == node->parent);
//     REQUIRE(node->parent->parent->children[1]);
//     REQUIRE(node->parent->parent->children[1]->get_bounds() == Dimension(0, 50, 100, 50));
// }
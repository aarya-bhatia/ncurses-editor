#include "catch.hpp"
#include "window/WindowManager.h"
#include "window/Window.h"
#include "window/ContentWindow.h"

struct TestContentView : public ContentWindow {

    TestContentView(Dimension bounds = Dimension()): ContentWindow(bounds) {}

    bool draw_called = false;
    bool show_called = false;
    bool resize_called = false;

    void reset() {
        draw_called = false;
        show_called = false;
        resize_called = false;
    }

    bool resize(Dimension bounds) override {
        Window::resize(bounds);
        resize_called = true;
        return true;
    }

    void draw() override {
        draw_called = true;
    }

    void show() override {
        show_called = true;
    }
};

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

    HSplitContainerWindow *container = new HSplitContainerWindow(bounds);
    container->add_child(new TestContentView(bounds));
    container->add_child(new TestContentView(bounds));
    container->arrange_children();

    REQUIRE(dynamic_cast<HSplitResizeStrategy *>(container->resize_strategy) != nullptr);

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
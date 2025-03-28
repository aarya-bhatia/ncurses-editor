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

TEST_CASE("window manager", "[windows]") {
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
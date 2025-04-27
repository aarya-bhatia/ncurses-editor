#include "catch.hpp"
#include "WindowManager.h"
#include "TestWindow.h"

#define MY_TEST_SUITE "[window_manager]"

TEST_CASE("draws all nodes", MY_TEST_SUITE) {
    Dimension d(0, 0, 100, 100);
    WindowManager <TestWindow*> wm(d);
    TestWindow* w = new TestWindow;
    wm.set_focused_node_content(w);
    wm.splith();
    REQUIRE(wm.focused_node->content == w);
}
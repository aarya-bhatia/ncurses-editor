#include "catch.hpp"
#include "WindowManager.h"
#include "TestWindow.h"

#define MY_TEST_SUITE "[window_manager]"

TEST_CASE("draws all nodes", MY_TEST_SUITE) {
    Dimension d(0, 0, 100, 100);
    WindowManager wm(d);
    TestWindow* w = new TestWindow();
    wm.get_current_tab()->set_focused_node_content(w);
    wm.get_current_tab()->splith();
    REQUIRE(wm.get_current_tab()->focused_node->content == w);
}
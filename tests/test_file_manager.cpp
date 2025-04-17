// #include "catch.hpp"
// #include "FileManager.h"
// #include "TestWindowManager.h"

// TEST_CASE("simple", "[files]") {
//     Dimension bounds(0, 0, 100, 100);
//     std::shared_ptr<IWindowManager> wm = std::shared_ptr<IWindowManager>(new TestWindowManager(bounds));
//     FileManager fm(wm);
//     REQUIRE(fm.count_files() == 0);
// }
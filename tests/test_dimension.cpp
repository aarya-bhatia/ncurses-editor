#include "catch.hpp"
#include "Dimension.h"

#define MY_TEST_SUITE "[dimension]"

TEST_CASE("Dimension initialization", MY_TEST_SUITE) {
    // Default constructor
    Dimension d1;
    REQUIRE(d1.x == 0);
    REQUIRE(d1.y == 0);
    REQUIRE(d1.width == 0);
    REQUIRE(d1.height == 0);
    
    // Constructor with parameters
    Dimension d2(10, 20, 30, 40);
    REQUIRE(d2.x == 10);
    REQUIRE(d2.y == 20);
    REQUIRE(d2.width == 30);
    REQUIRE(d2.height == 40);
    
    // Copy constructor
    Dimension d3(d2);
    REQUIRE(d3.x == 10);
    REQUIRE(d3.y == 20);
    REQUIRE(d3.width == 30);
    REQUIRE(d3.height == 40);
}

TEST_CASE("Dimension assignment operator", MY_TEST_SUITE) {
    Dimension d1(10, 20, 30, 40);
    Dimension d2;
    
    d2 = d1;
    REQUIRE(d2.x == 10);
    REQUIRE(d2.y == 20);
    REQUIRE(d2.width == 30);
    REQUIRE(d2.height == 40);
}

TEST_CASE("Dimension equality operators", MY_TEST_SUITE) {
    Dimension d1(10, 20, 30, 40);
    Dimension d2(10, 20, 30, 40);
    Dimension d3(5, 20, 30, 40);
    Dimension d4(10, 5, 30, 40);
    Dimension d5(10, 20, 5, 40);
    Dimension d6(10, 20, 30, 5);
    
    REQUIRE(d1 == d2);
    REQUIRE(!(d1 != d2));
    
    REQUIRE(d1 != d3);
    REQUIRE(d1 != d4);
    REQUIRE(d1 != d5);
    REQUIRE(d1 != d6);
}

TEST_CASE("Dimension contains", MY_TEST_SUITE) {
    Dimension outer(10, 10, 100, 100);
    
    // Completely inside
    Dimension inner1(20, 20, 50, 50);
    REQUIRE(outer.contains(inner1));
    
    // Same dimensions
    Dimension inner2(10, 10, 100, 100);
    REQUIRE(outer.contains(inner2));
    
    // Partially outside (x)
    Dimension outside1(5, 20, 50, 50);
    REQUIRE_FALSE(outer.contains(outside1));
    
    // Partially outside (y)
    Dimension outside2(20, 5, 50, 50);
    REQUIRE_FALSE(outer.contains(outside2));
    
    // Partially outside (width)
    Dimension outside3(20, 20, 100, 50);
    REQUIRE_FALSE(outer.contains(outside3));
    
    // Partially outside (height)
    Dimension outside4(20, 20, 50, 100);
    REQUIRE_FALSE(outer.contains(outside4));
    
    // Completely outside
    Dimension outside5(200, 200, 50, 50);
    REQUIRE_FALSE(outer.contains(outside5));
}

TEST_CASE("Dimension debug_string", MY_TEST_SUITE) {
    Dimension d(10, 20, 30, 40);
    REQUIRE(d.debug_string() == "10x20x30x40");
}

TEST_CASE("Dimension center", MY_TEST_SUITE) {
    Dimension d1(10, 20, 30, 40);
    Point center1 = d1.center();
    REQUIRE(center1.x == 25); // 10 + 30/2
    REQUIRE(center1.y == 40); // 20 + 40/2
    
    // Test with odd dimensions
    Dimension d2(10, 20, 31, 41);
    Point center2 = d2.center();
    REQUIRE(center2.x == 25); // 10 + 31/2 (integer division)
    REQUIRE(center2.y == 40); // 20 + 41/2 (integer division)
}

TEST_CASE("Point initialization and equality", MY_TEST_SUITE) {
    Point p1(10, 20);
    REQUIRE(p1.x == 10);
    REQUIRE(p1.y == 20);
    
    Point p2(10, 20);
    Point p3(5, 20);
    Point p4(10, 5);
    
    REQUIRE(p1 == p2);
    REQUIRE(p1 != p3);
    REQUIRE(p1 != p4);
}

TEST_CASE("Point distance_squared", MY_TEST_SUITE) {
    Point p1(10, 20);
    Point p2(13, 24);
    
    // Distance squared should be (13-10)^2 + (24-20)^2 = 9 + 16 = 25
    REQUIRE(p1.distance_squared(p2) == 25);
    REQUIRE(p2.distance_squared(p1) == 25);
    
    // Distance to self should be 0
    REQUIRE(p1.distance_squared(p1) == 0);
}
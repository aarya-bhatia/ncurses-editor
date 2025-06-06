#pragma once
#include <string>

struct Point
{
    unsigned x = 0;
    unsigned y = 0;

    Point(unsigned _x, unsigned _y) : x(_x), y(_y) {}

    float distance_squared(Point& other) const {
        float dy = other.y > y ? other.y - y : y - other.y;
        float dx = other.x > x ? other.x - x : x - other.x;
        return dy * dy + dx * dx;
    }

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point& other)const {
        return !operator==(other);
    }
};

struct Dimension
{
    unsigned int x;
    unsigned int y;
    unsigned int width;
    unsigned int height;

    Dimension(unsigned x = 0, unsigned y = 0, unsigned width = 0, unsigned height = 0) :
        x(x), y(y), width(width), height(height) {
    }

    Dimension(const Dimension& other) :
        x(other.x), y(other.y), width(other.width), height(other.height) {
    }

    Dimension& operator=(const Dimension& other) {
        if (this != &other) {
            x = other.x;
            y = other.y;
            width = other.width;
            height = other.height;
        }
        return *this;
    }

    bool operator!=(const Dimension& other) const {
        return x != other.x || y != other.y || width != other.width || height != other.height;
    }

    bool operator==(const Dimension& other) const {
        return x == other.x && y == other.y && width == other.width && height == other.height;
    }

    bool contains(const Dimension& other) const {
        return other.x >= x && other.y >= y && other.x + other.width <= width && other.y + other.height <= height;
    }

    std::string debug_string() const {
        return std::to_string(x) + "x" +
            std::to_string(y) + "x" +
            std::to_string(width) + "x" +
            std::to_string(height);
    }

    Point center() const {
        return Point(x + width / 2, y + height / 2);
    }

    bool empty() const { return width == 0 || height == 0; }
};
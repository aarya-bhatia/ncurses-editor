#pragma once

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
};
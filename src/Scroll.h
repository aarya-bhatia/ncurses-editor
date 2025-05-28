#pragma once
struct Scroll
{
    int dy = 0;
    int dx = 0;

    bool operator!=(const Scroll& other) const {
        return dy != other.dy && dx != other.dx;
    }

    bool operator==(const Scroll& other) const {
        return dy == other.dy && dx == other.dx;
    }
};

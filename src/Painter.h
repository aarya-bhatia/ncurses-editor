#pragma once

#include <string>
struct Painter
{
    virtual ~Painter() = default;
    virtual void draw_line(int y, const std::string& content) = 0;
    virtual void draw_till_eol(int y, int x, const std::string& content) = 0;
    virtual void draw_character(int y, int x, char c) = 0;
    virtual void clear() = 0;
    virtual void clear_line(int y) = 0;
    virtual void clear_till_eol(int y, int x) = 0;
    virtual void move(int y, int x) = 0;
    virtual void move_cursor(int y, int x) = 0;
};
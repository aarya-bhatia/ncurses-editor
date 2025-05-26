#pragma once
#include <list>
#include<string>

struct Cursor
{
    int y = 0;
    int x = 0;

    std::list<std::list<char>>::iterator line;
    std::list<char>::iterator col;

    std::string to_string() const {
        return "(" + std::to_string(y) + "," + std::to_string(x) + ")";
    }
};

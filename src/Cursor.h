#pragma once
#include <list>
struct Cursor
{
    int y;
    int x;

    std::list<std::list<char>>::iterator line;
    std::list<char>::iterator col;
};

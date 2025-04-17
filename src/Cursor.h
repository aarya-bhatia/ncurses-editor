#pragma once
#include <list>
struct Cursor
{
    int y = 0;
    int x = 0;

    std::list<std::list<char>>::iterator line;
    std::list<char>::iterator col;
};

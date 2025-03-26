#pragma once
#include <list>
#include <vector>

struct Cursor
{
    int y;
    int x;

    std::list<std::list<char>>::iterator line;
    std::list<char>::iterator col;
};

struct Scroll
{
    int dy;
    int dx;
};

struct File
{
    std::list<std::list<char>> lines;
    std::vector<Cursor> dirty_lines;

    Cursor cursor;
    Scroll scroll;

    bool force_redraw = false;

    char *filename;

    File(const char *filename);
    ~File();

    int load_file();
    int save_file();
};
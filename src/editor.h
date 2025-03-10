#pragma once

#include <list>
#include <string>
#include <assert.h>
#include "common.h"

#include <ncurses.h>

enum Mode
{
    NORMAL_MODE,
    INSERT_MODE,
    COMMAND_MODE
};

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

struct Editor
{
    std::list<std::list<char>> lines;

    WINDOW *edit_window = NULL;
    WINDOW *status_window = NULL;

    std::string mode_line = "";
    std::string statusline = "status";

    Cursor cursor;
    Scroll scroll;
    Mode mode = NORMAL_MODE;
    bool quit = false;

    Editor(const char *filename);
    ~Editor();

    void command(const std::string &command);

    void handle_command_mode_event(unsigned c);
    void handle_insert_mode_event(unsigned c);
    void handle_normal_mode_event(unsigned c);

    void draw();

    void cursor_up();
    void cursor_down();
    void cursor_left();
    void cursor_right();
};

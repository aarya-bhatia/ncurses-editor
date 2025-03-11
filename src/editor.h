#pragma once

#include <list>
#include <string>
#include <map>
#include <assert.h>
#include "common.h"

#include <ncurses.h>

enum Mode
{
    NORMAL_MODE,
    INSERT_MODE,
    COMMAND_MODE
};

static std::map<Mode, const char *> mode_names = {
    {NORMAL_MODE, "NORMAL"},
    {COMMAND_MODE, "COMMAND"},
    {INSERT_MODE, "INSERT"},
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
    WINDOW *console_window = NULL;

    std::string mode_line = "";
    std::string statusline = "status";

    Cursor cursor;
    Scroll scroll;
    Mode mode = NORMAL_MODE;
    bool quit = false;

    bool force_redraw = false;

    Editor(const char *filename);
    ~Editor();

    void command(const std::string &command);

    void handle_event(unsigned c);
    void handle_command_mode_event(unsigned c);
    void handle_insert_mode_event(unsigned c);
    void handle_normal_mode_event(unsigned c);

    void update();
    void draw();

    void cursor_up();
    void cursor_down();
    void cursor_left();
    void cursor_right();

    void move_cursor_eol();
    void scroll_to_ensure_cursor_visible();
    void force_redraw_editor();
};

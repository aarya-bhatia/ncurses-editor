#include "editor.h"
#include <curses.h>
#include <ncurses.h>
#include <string.h>
#include "log.h"
#include <sys/types.h>

Editor::Editor(const char *filename)
{
    log_info("No. lines: %d, cols: %d", LINES, COLS);
    edit_window = newwin(LINES - 2, COLS, 0, 0);
    status_window = newwin(2, COLS, LINES - 2, 0);

    refresh();

    std::list<std::string> str_lines = readlines(filename);

    werase(edit_window);
    // wprintw(edit_window, "test line");

    int y = 0;

    for (const std::string &str_line : str_lines)
    {
        lines.emplace_back(str_line.begin(), str_line.end());
        mvwprintw(edit_window, y, 0, "%s", str_line.substr(0, COLS).c_str());
        y++;
    }

    wrefresh(edit_window);

    if (lines.empty())
    {
        lines.push_back({});
    }

    cursor.x = 0;
    cursor.y = 0;
    cursor.line = lines.begin();
    cursor.col = lines.front().begin();

    scroll.dy = 0;
    scroll.dx = 0;

    move(cursor.y, cursor.x);
}

Editor::~Editor()
{
    delwin(edit_window);
    delwin(status_window);
}

void Editor::cursor_up()
{
    if (cursor.y > 0)
    {
        cursor.y--;
        cursor.line--;

        std::list<char> &line_val = *cursor.line;
        if (cursor.x >= line_val.size())
        {
            cursor.x = line_val.size() - 1;
        }

        cursor.col = line_val.begin();
        std::advance(cursor.col, cursor.x);
    }
}

void Editor::cursor_down()
{
    if (cursor.y < lines.size() - 1)
    {
        cursor.y++;
        cursor.line++;

        std::list<char> &line_val = *cursor.line;
        if (cursor.x >= line_val.size())
        {
            cursor.x = line_val.size() - 1;
        }

        cursor.col = line_val.begin();
        std::advance(cursor.col, cursor.x);
    }
}

void Editor::cursor_left()
{
    if (cursor.x > 0)
    {
        cursor.x--;
        cursor.col--;
    }
}

void Editor::cursor_right()
{
    std::list<char> &line_val = *cursor.line;
    if (line_val.size() > 0 && cursor.x < line_val.size() - 1)
    {
        cursor.x++;
        cursor.col++;
    }
}

void Editor::command(const std::string &command)
{
    if (command == "q")
    {
        quit = true;
    }
    else
    {
        statusline = "no such command";
    }
}

void Editor::handle_insert_mode_event(unsigned c)
{
    std::list<char> &line_val = *cursor.line;

    switch (c)
    {
    case CTRL_ESCAPE:
        if (line_val.size() > 0 && cursor.x >= line_val.size())
        {
            cursor.x = line_val.size() - 1;
            cursor.col = --line_val.end();
        }

        mode = NORMAL_MODE;
        break;

    default:
        if (PRINTABLE(c))
        {
            cursor.col = line_val.insert(cursor.col, c);
            cursor.x++;
        }
    }
}

void Editor::handle_normal_mode_event(unsigned c)
{
    switch (c)
    {
    case 'h':
        cursor_left();
        break;

    case 'l':
        cursor_right();
        break;

    case 'j':
        cursor_down();
        break;

    case 'k':
        cursor_up();
        break;

    case CTRL_ESCAPE:
        statusline = "";
        break;

    case '0':
        cursor.x = 0;
        break;

    case 'i':
        mode = INSERT_MODE;
        break;

    case ':':
        mode = COMMAND_MODE;
        mode_line = "";
        break;
    }
}

void Editor::handle_command_mode_event(unsigned c)
{
    switch (c)
    {
    case CTRL_ENTER:
        mode = NORMAL_MODE;
        command(mode_line);
        break;

    default:
        if (PRINTABLE(c))
        {
            mode_line += c;
        }
    }
}

void Editor::draw()
{
    werase(status_window);
    wprintw(status_window, statusline.c_str());
    wrefresh(status_window);

    move(cursor.y, cursor.x);
}

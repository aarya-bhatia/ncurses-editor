#include "editor.h"
#include "edit_buffer.h"
#include <curses.h>
#include <string.h>

#define PRINTABLE(c) ((c) > 0x1f && (c) < 0x7f)

View view_edit;
View view_mode;
View view_status;
int editor_mode;
Vec2 cursor;

void handle_command(const char *command)
{
    if (strcmp(command, "q") == 0) {
        destroy();
        exit(0);
    }
}

void draw_view_edit()
{
    werase(view_edit.window);
    edit_buffer_print_window(&view_edit.line, view_edit.window);
    wrefresh(view_edit.window);
}

void draw_view_mode()
{
    werase(view_mode.window);

    if (editor_mode == INSERT_MODE) {
        wprintw(view_mode.window, "-- INSERT --");
    } else if (editor_mode == NORMAL_MODE) {
        wprintw(view_mode.window, "-- NORMAL --");
    } else if (editor_mode == COMMAND_MODE) {
        wprintw(view_mode.window, ":%s", edit_buffer_to_string(&view_mode.line));
    }

    wrefresh(view_mode.window);
}

void draw_view_status(const char *format, ...)
{
    werase(view_status.window);

    if (format) {
        va_list args;
        va_start(args, format);
        vw_printw(view_status.window, format, args);
        va_end(args);
    }

    wrefresh(view_status.window);
}

void draw_cursor()
{
    move(cursor.y, cursor.x);
}

void insert_mode_key_event(unsigned c)
{
    switch (c) {
        case CTRL_ESCAPE:
            edit_buffer_close_gap(&view_edit.line);
            editor_mode = NORMAL_MODE;
            break;

        case CTRL_BACKSPACE:
        case CTRL_DEL:
        case KEY_BACKSPACE:
        case KEY_DC:
            edit_buffer_backspace(&view_edit.line);
            cursor.x--;
            break;

        case CTRL_U:
        case CTRL_ENTER:
            edit_buffer_clear(&view_edit.line);
            cursor.x = 0;
            break;

        case CTRL_A:
            edit_buffer_open_gap(&view_edit.line, 0, 1);
            cursor.x = 0;
            break;

        case CTRL_E:
            edit_buffer_close_gap(&view_edit.line);
            edit_buffer_open_gap(&view_edit.line, edit_buffer_size(&view_edit.line), 1);
            cursor.x = edit_buffer_size(&view_edit.line) - 1;
            break;

        default:
            if (PRINTABLE(c)) {
                edit_buffer_insert(&view_edit.line, c);
                cursor.x++;
            }
    }

    draw_view_status("key pressed: 0x%02x", c);
}

void normal_mode_key_event(unsigned c)
{
    switch (c) {
        case CTRL_ESCAPE:
            draw_view_status(NULL); // clear status
			break;

        case 'i':
            edit_buffer_open_gap(&view_mode.line, cursor.x, 1);
            editor_mode = INSERT_MODE;
            draw_view_status("leave normal mode");
            break;

        case 'h':
            if (cursor.x > 0) {
                cursor.x--;
            }
            draw_view_status("move left");
            break;

        case 'l':
            if (cursor.x < edit_buffer_size(&view_edit.line)) {
                cursor.x++;
            }
            draw_view_status("move right");
            break;

        case ':':
            editor_mode = COMMAND_MODE;
            edit_buffer_clear(&view_mode.line);
            draw_view_status("leave normal mode");
            break;
    }
}

void command_mode_key_event(unsigned c)
{
    switch (c) {
        case CTRL_ENTER:
            editor_mode = NORMAL_MODE;
            handle_command(edit_buffer_to_string(&view_mode.line));
            break;

        default:
            if (PRINTABLE(c)) {
                edit_buffer_insert(&view_mode.line, c);
            }
    }
}

void update()
{
    draw_view_mode();
    draw_view_edit();
    draw_cursor();
}

void init()
{
    initscr();
    raw();
    noecho();
    notimeout(stdscr, TRUE);
    // keypad(stdscr, TRUE);
    // scrollok(stdscr, TRUE);

    cursor.x = 0;
    cursor.y = 0;

    editor_mode = NORMAL_MODE;

    memset(&view_edit, 0, sizeof view_edit);
    memset(&view_mode, 0, sizeof view_edit);
    memset(&view_status, 0, sizeof view_edit);

    view_edit.window = newwin(LINES / 2, COLS, 0, 0);
    view_mode.window = newwin(2, COLS / 2, LINES - 2, 0);
    view_status.window = newwin(2, COLS / 2, LINES - 2, COLS / 2);

    refresh();
    update();
}

void destroy()
{
    edit_buffer_free(&view_edit.line);
    edit_buffer_free(&view_mode.line);
    edit_buffer_free(&view_status.line);

    delwin(view_edit.window);
    delwin(view_mode.window);
    delwin(view_status.window);

    endwin();
}

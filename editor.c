#include "editor.h"
#include "edit_buffer.h"
#include <curses.h>
#include <ncurses.h>
#include <string.h>
#include <sys/types.h>
#include "log.h"

#define PRINTABLE(c) ((c) > 0x1f && (c) < 0x7f)

View view_edit;
View view_mode;
View view_status;
int editor_mode;
Vec2 cursor;

void print_buffer(EditBuffer *b, WINDOW *win)
{
    werase(win);

    for (EditNode *node = b->head; node != NULL; node = node->next) {
        wprintw(win, "%s", node->buffer);
    }

    wrefresh(win);
}

void handle_command(const char *command)
{
    if (strcmp(command, "q") == 0) {
        destroy();
        exit(0);
    }
}

void draw_view_edit()
{
    print_buffer(&view_edit.line, view_edit.window);
}

void draw_view_mode()
{
    werase(view_mode.window);

    if (editor_mode == INSERT_MODE) {
        wprintw(view_mode.window, "-- INSERT --");
    } else if (editor_mode == NORMAL_MODE) {
        wprintw(view_mode.window, "-- NORMAL --");
    } else if (editor_mode == COMMAND_MODE) {
        if (view_mode.line.head == NULL) {
            wprintw(view_mode.window, ":");
        } else {
            wprintw(view_mode.window, ":%s", view_mode.line.head->buffer);
        }
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
            on_insert_leave();
            on_normal_enter();
            break;

        case CTRL_BACKSPACE:
        case CTRL_DEL:
        case KEY_BACKSPACE:
        case KEY_DC:
            // TODO
            break;

        case CTRL_U:
            // TODO
            break;

        case CTRL_ENTER:
            edit_buffer_clear(&view_edit.line);
            cursor.x = 0;
            edit_buffer_set_insert_position(&view_edit.line, 0);
            break;

        case CTRL_A:
            edit_buffer_set_insert_position(&view_edit.line, 0);
            cursor.x = 0;
            break;

        case CTRL_E:
            edit_buffer_set_insert_position(&view_edit.line, edit_buffer_size(&view_edit.line));
            cursor.x = edit_buffer_size(&view_edit.line);
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
            on_normal_leave();
            on_insert_enter();
            break;

        case 'h':
            if (cursor.x > 0) {
                cursor.x--;
            }
            break;

        case 'l':
            if (edit_buffer_size(&view_edit.line) && cursor.x < edit_buffer_size(&view_edit.line) - 1) {
                cursor.x++;
            }
            break;

        case ':':
            on_normal_leave();
            on_command_enter();
            break;
    }
}

void command_mode_key_event(unsigned c)
{
    switch (c) {
        case CTRL_ENTER:
            editor_mode = NORMAL_MODE;
            on_command_leave();
            on_normal_enter();
            break;

        default:
            if (PRINTABLE(c)) {
                edit_buffer_insert(&view_mode.line, c);
            }
    }
}

void update()
{
    if (editor_mode == COMMAND_MODE) {
        draw_view_mode();
    }

    if (editor_mode == INSERT_MODE) {
        draw_view_edit();
    }

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

    edit_buffer_init(&view_edit.line);
    edit_buffer_init(&view_mode.line);
    edit_buffer_init(&view_status.line);

    view_edit.window = newwin(LINES / 2, COLS, 0, 0);
    view_mode.window = newwin(2, COLS / 2, LINES - 2, 0);
    view_status.window = newwin(2, COLS / 2, LINES - 2, COLS / 2);

    refresh();

	draw_view_mode();
	draw_cursor();
}

void destroy()
{
    edit_buffer_clear(&view_edit.line);
    edit_buffer_clear(&view_mode.line);
    edit_buffer_clear(&view_status.line);

    delwin(view_edit.window);
    delwin(view_mode.window);
    delwin(view_status.window);

    endwin();
}

void on_insert_enter()
{
    editor_mode = INSERT_MODE;
    log_debug("enter insert mode");
    draw_view_mode();

    cursor.x = MAX(0, MIN(cursor.x, edit_buffer_size(&view_edit.line) - 1));
    cursor.y = MAX(cursor.y, 0);

    edit_buffer_set_insert_position(&view_edit.line, cursor.x);
}

void on_insert_leave()
{
    log_debug("leave insert mode");
}

void on_normal_enter()
{
    editor_mode = NORMAL_MODE;
    log_debug("enter normal mode");
    draw_view_mode();
    if (edit_buffer_size(&view_edit.line) > 0 && cursor.x >= edit_buffer_size(&view_edit.line)) {
        cursor.x = edit_buffer_size(&view_edit.line) - 1;
    }
}

void on_normal_leave()
{
    log_debug("leave normal mode");
}

void on_command_enter()
{
    editor_mode = COMMAND_MODE;
    log_debug("enter command mode");
    draw_view_mode();
    edit_buffer_clear(&view_mode.line);
    edit_buffer_set_insert_position(&view_mode.line, 0);
}

void on_command_leave()
{
    log_debug("leave command mode");
    handle_command(edit_buffer_to_string(&view_mode.line));
}

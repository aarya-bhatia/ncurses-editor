#include "edit_buffer.h"
#include "editor.h"
#include "log.h"
#include <curses.h>
#include <ncurses.h>
#include <sys/types.h>

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

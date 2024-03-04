#include "editor.h"
#include "edit_buffer.h"

void editor_draw(Editor *editor, const char *status_format, ...) {
  werase(editor->win_status);
  if (editor->mode == INSERT_MODE) {
    wprintw(editor->win_status, "-- INSERT --");
  } else if (editor->mode == NORMAL_MODE) {
    wprintw(editor->win_status, "-- NORMAL --");
  }
  if (status_format) {
    va_list args;
    va_start(args, status_format);
    wprintw(editor->win_status, "\t\t");
    vw_printw(editor->win_status, status_format, args);
    va_end(args);
  }
  wrefresh(editor->win_status);

  edit_buffer_print_window(&editor->line, editor->win_editor);
  wmove(editor->win_editor, editor->cursor.y, editor->cursor.x);
}

void key_handler(KeyEvent ev, void *args) {
  Editor *editor = (Editor *)args;
  unsigned c = ev.keys[0];

  if (editor->mode == INSERT_MODE) {
    switch (c) {
    case CTRL_ESCAPE:
      edit_buffer_close_gap(&editor->line);
      editor->mode = NORMAL_MODE;
      break;

    case CTRL_BACKSPACE:
    case CTRL_DEL:
    case KEY_BACKSPACE:
    case KEY_DC:
      edit_buffer_backspace(&editor->line);
      editor->cursor.x--;
      break;

    case CTRL_U:
      edit_buffer_clear(&editor->line);
      editor->cursor.x = 0;
      break;

    case CTRL_A:
      edit_buffer_open_gap(&editor->line, 0, 1);
      editor->cursor.x = 0;
      break;

    case CTRL_E:
      edit_buffer_close_gap(&editor->line);
      edit_buffer_open_gap(&editor->line, edit_buffer_size(&editor->line), 1);
      editor->cursor.x = edit_buffer_size(&editor->line) - 1;
      break;

    case CTRL_C:
      editor_free(editor);
      exit(0);

    case CTRL_ENTER:
      edit_buffer_insert(&editor->line, c);
      editor->cursor.x = 0;
      editor->cursor.y++;
      break;

    default:
      if (c > 0x1f && c < 0x7f) {
        edit_buffer_insert(&editor->line, c);
        editor->cursor.x++;
      }
    }

  } else if (editor->mode == NORMAL_MODE) {
    switch (c) {
    case 'i':
      editor->mode = INSERT_MODE;
      break;
    }
  }

  editor_draw(editor, "key pressed: 0x%02x", c);
}

Editor *editor_alloc() {
  Editor *editor = calloc(1, sizeof *editor);
  editor->mode = INSERT_MODE;
  editor->win_editor = newwin(LINES - 2, COLS, 0, 0);
  editor->win_status = newwin(0, 0, LINES - 2, 0);
  wrefresh(editor->win_editor);
  wrefresh(editor->win_status);
  editor->keyboard.win_editor = editor->win_editor;
  editor->keyboard.key_handler = key_handler;
  editor->keyboard.args = editor;
  wmove(editor->win_editor, 0, 0);
  editor->cursor.x = 0;
  editor->cursor.y = 0;
  return editor;
}

void editor_start(Editor *editor) {
  pthread_create(&editor->key_listener, NULL, start_key_listener,
                 &editor->keyboard);
  pthread_join(editor->key_listener, NULL);
}

void editor_free(Editor *editor) {
  if (!editor) {
    return;
  }

  edit_buffer_free(&editor->line);

  delwin(editor->win_editor);
  delwin(editor->win_status);
  endwin();

  free(editor);
}

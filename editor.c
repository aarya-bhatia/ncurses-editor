#include "editor.h"
#include "edit_buffer.h"
#include <assert.h>

Editor *editor_alloc() {
  Editor *editor = calloc(1, sizeof *editor);
  editor->mode = INSERT_MODE;
  editor->win_editor = newwin(LINES - 2, COLS, 0, 0);
  editor->win_status = NULL;
  // newwin(0, 0, LINES - 2, 0);
  wrefresh(editor->win_editor);
  wrefresh(editor->win_status);
  return editor;
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

void editor_draw(Editor *editor) {
  assert(editor);
  edit_buffer_print_window(&editor->line, editor->win_editor);
}

int editor_input(Editor *editor) {
  int c = wgetch(editor->win_editor);
  edit_buffer_insert(&editor->line, c);
  if (c == 'q') {
    return -1;
  }

  return 0;
}

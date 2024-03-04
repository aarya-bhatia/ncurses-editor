#pragma once

#include "edit_buffer.h"
#include "vec2.h"
#include <ncurses.h>

#define KEY_ESCAPE 27

enum { NORMAL_MODE, INSERT_MODE, COMMAND_MODE };

typedef struct Editor {
  WINDOW *win_status;
  WINDOW *win_editor;
  EditBuffer line;
  Vec2 cursor;
  int mode;
} Editor;

Editor *editor_alloc();
void editor_free(Editor *editor);
void editor_draw(Editor *editor);
int editor_input(Editor *editor);

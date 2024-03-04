#pragma once

#include "edit_buffer.h"
#include "keyboard.h"
#include "vec2.h"
#include <ncurses.h>

enum { NORMAL_MODE, INSERT_MODE, COMMAND_MODE };

typedef struct Editor {
  WINDOW *win_status;
  WINDOW *win_editor;
  EditBuffer line;
  Keyboard keyboard;
  Vec2 cursor;
  int mode;
  pthread_t key_listener;
} Editor;

Editor *editor_alloc();
void editor_free(Editor *editor);
void editor_start(Editor *editor);

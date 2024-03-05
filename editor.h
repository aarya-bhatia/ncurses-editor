#pragma once

#include "edit_buffer.h"
#include "keyboard.h"
#include "vec2.h"
#include <ncurses.h>

enum { NORMAL_MODE, INSERT_MODE, COMMAND_MODE };

typedef struct View {
    WINDOW *window;
    EditBuffer line;
} View;

void init();
void destroy();
void update();

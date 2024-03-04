#include "editor.h"
#include <ncurses.h>

int main() {
  initscr();
  raw();
  noecho();
  notimeout(stdscr, true);
  keypad(stdscr, true);

  Editor *editor = editor_alloc();

  editor_start(editor);

  // if (mode == INSERT_MODE) {
  //   if (c == KEY_ESCAPE) {
  //     mode = NORMAL_MODE;
  //   } else if (c == KEY_BACKSPACE) {
  //     input[strlen(input) - 1] = 0;
  //     x--;
  //   } else if (c == '\n' || c == ('u' & 0x1f)) {
  //     input[0] = 0;
  //     x = 0;
  //   } else {
  //     size_t n = strlen(input);
  //     if (n < sizeof(input) - 1) {
  //       input[n] = c;
  //       input[n + 1] = 0;
  //     }
  //     x++;
  //   }
  // } else {
  //   if (c == 'i') {
  //     mode = INSERT_MODE;
  //     x = strlen(input);
  //   } else if ((c == 'h' || c == KEY_LEFT) && x > 0) {
  //     x--;
  //   } else if ((c == 'l' || c == KEY_RIGHT) && x < COLS - 1) {
  //     x++;
  //   }
  // }

  editor_free(editor);
  return 0;
}

#include <ncurses.h>

int main() {
  initscr();
  keypad(stdscr, true);

  printw("hello world");
  refresh();

  while (1) {
    int y, x;
    getyx(stdscr, y, x);

    int c = getch();
    switch (c) {
    case KEY_LEFT:
      x--;
      break;
    case KEY_RIGHT:
      x++;
      break;
    case KEY_UP:
      y--;
      break;
    case KEY_DOWN:
      y++;
      break;
    }

    if (y < 0) {
      y = 0;
    }

    if (x < 0) {
      x = 0;
    }

    if (x >= COLS) {
      x = COLS - 1;
    }

    if (y >= LINES) {
      y = LINES - 1;
    }

    move(y, x);
    refresh();
  }

  endwin();
  return 0;
}

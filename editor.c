#include <ncurses.h>
#include <string.h>

#define KEY_ESCAPE 27

enum { NORMAL_MODE, INSERT_MODE };

int mode = INSERT_MODE;
char input[1024] = {0};
int y = 0, x = 0;

int main() {
  initscr();
  cbreak();
  noecho();
  notimeout(stdscr, true);
  keypad(stdscr, true);

  do {
    erase();
    printw("%s", input);
    move(y, x);
    refresh();

    int c = getch();

    if (mode == INSERT_MODE) {
      if (c == KEY_ESCAPE) {
        mode = NORMAL_MODE;
      } else if (c == KEY_BACKSPACE) {
        input[strlen(input) - 1] = 0;
        x--;
      } else if (c == '\n' || c == ('u'&0x1f)) {
        input[0] = 0;
        x = 0;
      } else {
        size_t n = strlen(input);
        if (n < sizeof(input) - 1) {
          input[n] = c;
          input[n + 1] = 0;
        }
        x++;
      }
    } else {
      if (c == 'i') {
        mode = INSERT_MODE;
        x = strlen(input);
      } else if ((c == 'h' || c == KEY_LEFT) && x > 0) {
        x--;
      } else if ((c == 'l' || c == KEY_RIGHT) && x < COLS - 1) {
        x++;
      }
    }
  } while (1);

  endwin();
}

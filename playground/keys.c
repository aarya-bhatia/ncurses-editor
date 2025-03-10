#include <ncurses.h>
#include <stdbool.h>
#include <unistd.h>

void keylistener()
{
    char keys[100] = {0};
    int i = 0;

    while (1) {
        int ch = getch();

        if (ch == 27) {
            printw("%s:", "ESC");
        }

        if (ch == KEY_F(1)) {
            printw("F1:");
        } else if (ch == KEY_LEFT) {
            printw("LEFT:");
        }

        printw("%c ", ch);
        // keys[i++] = ch;
        // move(0, 0);
        // clrtoeol();
        // printw("%s", keys);
        refresh();
    }
}

int main()
{
    initscr();
    refresh();
    noecho();
    cbreak();
    keypad(stdscr, true);
    // notimeout(stdscr, true);
	set_escdelay(80);
    keylistener();
    endwin();
    return 0;
}

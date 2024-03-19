#include <ncurses.h>
#include <unistd.h>

int main()
{
    initscr();
    keypad(stdscr, true);
    refresh();

    timeout(500);

    char keys[10];
    int i = 0;

    while (1) {
        int ch = getch();
        if (ch == ERR) {
            break;
        }
        keys[i++] = ch;
    }

    keys[i] = 0;

    printw("the keys are: %s\n", keys);
    refresh();

    timeout(-1);
    getch();
    endwin();
    return 0;
}

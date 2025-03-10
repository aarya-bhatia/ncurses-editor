#include <ncurses.h>
#include <unistd.h>

int main()
{
    initscr();
    keypad(stdscr, true);

    printw("world");
    move(0, 0);
    printw("hi ");
    refresh();

    getch();

    endwin();
    return 0;
}

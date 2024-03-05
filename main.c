#include "editor.h"
#include <ncurses.h>
#include <string.h>

extern int editor_mode;

void insert_mode_key_event(unsigned);
void normal_mode_key_event(unsigned);
void command_mode_key_event(unsigned);

int main()
{
    init();
    notimeout(stdscr, TRUE);

    while (1) {
        int c = getch();

        if (c == CTRL_C) {
            destroy();
            exit(0);
        }

        if (editor_mode == INSERT_MODE) {
            insert_mode_key_event(c);
        } else if (editor_mode == NORMAL_MODE) {
            normal_mode_key_event(c);
        } else if (editor_mode == COMMAND_MODE) {
            command_mode_key_event(c);
        }

        update();
    }

    destroy();
    return 0;
}

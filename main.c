#include "editor.h"
#include "log.h"
#include <fcntl.h>
#include <unistd.h>

extern int editor_mode;

void insert_mode_key_event(unsigned);
void normal_mode_key_event(unsigned);
void command_mode_key_event(unsigned);

int main()
{
    init();
    notimeout(stdscr, TRUE);

    int logfile = open("stderr.log", O_CREAT | O_TRUNC | O_WRONLY, 0640);
    dup2(logfile, 2);
    close(logfile);
    log_info("hello world");

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

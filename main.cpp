#include "src/editor.h"
#include "src/log.h"
#include <fcntl.h>
#include <unistd.h>

void setup_logger()
{
    int logfile = open("stderr.log", O_CREAT | O_TRUNC | O_WRONLY, 0640);
    dup2(logfile, 2);
    close(logfile);
    log_info("hello world");
}

int main()
{
    setup_logger();

    initscr();            // Start ncurses mode
    noecho();             // Don't echo typed characters
    cbreak();             // Disable line buffering (input is available immediately)
    keypad(stdscr, TRUE); // Enable special keys (e.g., arrows)
    refresh();

    Editor editor("Makefile");
    editor.draw();

    while (!editor.quit)
    {
        int c = getch();

        if (c == CTRL_C)
        {
            editor.quit = true;
        }
        else if (editor.mode == INSERT_MODE)
        {
            editor.handle_insert_mode_event(c);
        }
        else if (editor.mode == NORMAL_MODE)
        {
            editor.handle_normal_mode_event(c);
        }
        else if (editor.mode == COMMAND_MODE)
        {
            editor.handle_command_mode_event(c);
        }

        editor.draw();
    }

    endwin();

    return 0;
}

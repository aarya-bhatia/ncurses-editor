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

void init()
{
    initscr();            // Start ncurses mode
    noecho();             // Don't echo typed characters
    cbreak();             // Disable line buffering (input is available immediately)
    keypad(stdscr, TRUE); // Enable special keys (e.g., arrows)
    refresh();
}

void cleanup()
{
    endwin();
}

int main()
{
    setup_logger();
    init();

    Editor editor("Makefile");
    editor.draw();

    while (!editor.quit)
    {
        int c = getch();
        editor.handle_event(c);

        // log_debug("cursor: x:%d y:%d, scroll: dx:%d dy:%d", editor.cursor.x,
        //         editor.cursor.y, editor.scroll.dx, editor.scroll.dy);
    }

    cleanup();
    return 0;
}

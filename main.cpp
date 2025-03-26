#include "src/editor.h"
#include "src/log.h"
#include <fcntl.h>
#include <unistd.h>

bool resized = false;

void setup_logger()
{
    log_set_level(LOG_DEBUG);
    int logfile = open("stderr.log", O_CREAT | O_TRUNC | O_WRONLY, 0640);
    dup2(logfile, 2);
    close(logfile);
    log_info("BEGIN");
}

void init()
{
    setup_logger();

    initscr();             // Start ncurses mode
    noecho();              // Don't echo typed characters
    cbreak();              // Disable line buffering (input is available immediately)
    keypad(stdscr, FALSE); // Don't handle special keys to fix escape key delay issue
}

void init_screen()
{
    endwin();  // Reset ncurses
    refresh(); // Refresh stdscr
    clear();   // Clear the screen
}

void cleanup()
{
    endwin();
}

void handle_resize(int sig)
{
    if (sig == SIGWINCH)
    {
        resized = true;
    }
}

int main()
{
    init();
    init_screen();

    signal(SIGWINCH, handle_resize);

    Editor editor;
    editor.draw();

    editor.open({"test.txt", "Makefile", "main.cpp"});
    log_info("Active file: %s", editor.get_current_filename());

    while (!editor.quit)
    {
        if (resized)
        {
            resized = false;
            log_info("window resize detected!");
            init_screen();
            editor.resize();
            continue;
        }

        int c = getch();
        editor.handle_event(c);
        editor.update();

        // log_debug("cursor: x:%d y:%d, scroll: dx:%d dy:%d", editor.cursor.x,
        //           editor.cursor.y, editor.scroll.dx, editor.scroll.dy);
    }

    cleanup();
    return 0;
}

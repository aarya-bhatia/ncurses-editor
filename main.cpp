#include "log.h"
#include <fcntl.h>
#include <unistd.h>
#include <ncurses.h>
#include "window/WindowManager.h"
#include "application/editor.h"
#include <memory>

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
    editor.open({"main.cpp"});

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

        editor.draw();
        editor.show();

        int ch = getch();

        if (ch == CTRL_C) {
            break;
        }

        editor.handle_event(ch);
    }

    cleanup();
    return 0;
}

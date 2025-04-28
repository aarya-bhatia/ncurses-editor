#include "util/log.h"
#include <fcntl.h>
#include <unistd.h>
#include <ncurses.h>
#include "Editor.h"
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
    refresh();

    log_info("screen size: %dx%d", getmaxx(stdscr), getmaxy(stdscr));
}

void init_screen()
{
    endwin();  // Reset ncurses
    refresh(); // Refresh stdscr
    clear();   // Clear the screen
    refresh(); // Refresh stdscr

    log_info("screen size: %dx%d", getmaxx(stdscr), getmaxy(stdscr));
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

int main(int argc, const char** argv)
{
    init();
    signal(SIGWINCH, handle_resize);

    std::vector<std::string> filenames;
    for (const char** arg = argv + 1; *arg != NULL; arg++) {
        filenames.push_back(std::string(*arg));
    }

    Editor editor(Dimension(0, 0, COLS, LINES));
    editor.open(filenames);

    while (!editor.quit)
    {
        if (resized)
        {
            log_info("window resize detected!");
            resized = false;
            init_screen();
            editor.resize(Dimension(0, 0, COLS, LINES));
            continue;
        }

        // log_debug("drawing editor");
        editor.draw();
        editor.show();

        // log_debug("getch");
        int ch = getch();

        if (ch == CTRL_C) {
            break;
        }

        // log_debug("handling event");
        editor.handle_event(ch);
    }

    cleanup();
    return 0;
}

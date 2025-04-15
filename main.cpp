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

int main(int argc, const char** argv)
{
    init();
    init_screen();
    signal(SIGWINCH, handle_resize);

    std::vector<std::string> filenames;
    for (const char** arg = argv + 1; *arg != NULL; arg++) {
        filenames.push_back(std::string(*arg));
    }

    Editor editor;
    editor.open(filenames);

    if (editor.file_manager->count_files() == 0) {
        auto new_file = editor.file_manager->open_untitled_file();
        editor.file_manager->open_in_current_window(new_file);
    }

    assert(editor.file_manager->count_files() > 0);

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

        log_info("total window nodes: %d", editor.window_manager->count_nodes());
        log_info("total content nodes: %d", editor.window_manager->count_content_nodes());
    }

    cleanup();
    return 0;
}

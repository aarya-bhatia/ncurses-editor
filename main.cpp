#include "log.h"
#include <fcntl.h>
#include <unistd.h>
#include <ncurses.h>
#include "window/WindowManager.h"
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

struct TestContentView : public ContentView {

    WINDOW* win = nullptr;

    TestContentView() {
    }

    ~TestContentView() {
        if (win) {
            delwin(win);
        }
    }

    std::unique_ptr<ContentView> clone() override {
        return std::unique_ptr<ContentView>(new TestContentView());
    }

    void resize(Dimension bounds) override {
        if (win) {
            delwin(win);
        }

        log_info("created new window");
        win = newwin(bounds.height, bounds.width, bounds.y, bounds.x);
        refresh();
    }

    void draw() override {
        log_info("drawing test content");
        wclear(win);
        mvwprintw(win, 0, 0, "Test");
        wmove(win, 1, 2);
    }

    void show() override {
        wrefresh(win);
        log_info("showing test content");
    }
};

int main()
{
    init();
    init_screen();

    signal(SIGWINCH, handle_resize);

    Dimension screen_bounds(0, 0, (unsigned)COLS, (unsigned)LINES);
    WindowManager wm(screen_bounds);

    ContentWindow* current_window = wm.get_content_node();

    wm.split_horizontal();
    current_window = wm.get_content_node();
    current_window->set_view(std::unique_ptr<ContentView>(new TestContentView()));

    int ch = 0;
    while (ch != 'q') {
        wm.draw();
        wm.show();
        ch = getch();
    }

    cleanup();
    return 0;
}

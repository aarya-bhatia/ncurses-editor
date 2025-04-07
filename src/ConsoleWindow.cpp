#include "ConsoleWindow.h"
#include "application/editor.h"

void ConsoleWindow::draw() {
    wclear(window.get());

    int ncols = getmaxx(window.get());

    if (editor.mode == COMMAND_MODE)
    {
        std::string tmp = ":" + editor.mode_line;
        if (tmp.size() >= ncols)
        {
            tmp = tmp.substr(tmp.size() - ncols - 1);
        }

        wprintw(window.get(), tmp.substr(0, ncols).c_str());
    }
    else
    {
        wprintw(window.get(), editor.statusline.substr(0, ncols).c_str());
    }
}

void ConsoleWindow::show() {
    wrefresh(window.get());
}

void ConsoleWindow::resize(Dimension bounds) {
    this->bounds = bounds;
    this->window = NcursesWindow(bounds);
}
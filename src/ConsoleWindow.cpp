#include "ConsoleWindow.h"
#include "Editor.h"

void ConsoleWindow::draw() {
    wclear(window.get());

    int ncols = getmaxx(window.get());

    if (editor.editor_mode)
    {
        std::string tmp = editor.editor_mode->get_console_string();
        wprintw(window.get(), tmp.substr(0, ncols).c_str());
    }
}

void ConsoleWindow::show() {
    wrefresh(window.get());
}

void ConsoleWindow::resize(Dimension bounds) {
    this->bounds = bounds;
    this->window = NcursesWindow(bounds);
}
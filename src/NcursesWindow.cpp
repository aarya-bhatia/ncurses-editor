#include "NcursesWindow.h"

void NcursesWindow::draw_line(int y, const std::string& content) {
    if (is_valid_position(y, 0)) {
        mvwprintw(window.get(), y, 0, content.substr(0, width()).c_str());
    }
}

void NcursesWindow::draw_till_eol(int y, int x, const std::string& content) {
    if (is_valid_position(y, x)) {
        mvwprintw(window.get(), y, x, content.substr(0, width() - x).c_str());
    }
}

void NcursesWindow::draw_character(int y, int x, char c) {
    if (is_valid_position(y, x)) {
        mvwaddch(window.get(), y, x, c);
    }
}

void NcursesWindow::clear() {
    wclear(window.get());
}

void NcursesWindow::clear_line(int y) {
    wmove(window.get(), y, 0);
    wclrtoeol(window.get());
}

void NcursesWindow::clear_till_eol(int y, int x) {
    wmove(window.get(), y, x);
    wclrtoeol(window.get());
}

void NcursesWindow::move(int y, int x) {
    wmove(window.get(), y, x);
}
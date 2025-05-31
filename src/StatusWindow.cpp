#include "Editor.h"
#include "StatusWindow.h"
#include <sstream>

void StatusWindow::draw()
{
    werase(win);
    mvwprintw(win, 0, 0, "%s", get_status().c_str());
    wnoutrefresh(win);
}

void StatusWindow::resize(Dimension bounds)
{
    werase(win);
    wnoutrefresh(win);
    delwin(win);
    this->bounds = bounds;
    this->win = newwin(bounds.height, bounds.width, bounds.y, bounds.x);
}

std::string StatusWindow::get_status()
{
    std::ostringstream left_oss;

    File* file = editor.get_focused_file();

    left_oss << editor.mode_name();

    int tabno = editor.window_manager.get_tab_index() + 1;
    int ntabs = editor.window_manager.count_tabs();
    left_oss << " | tab " << tabno << " of " << ntabs;

    if (file)
    {
        if (!file->filename.empty()) {
            left_oss << " | " << file->filename;
        }

        left_oss << " | " << "Ln:" << 1 + file->cursor.y
            << "/" << file->lines.size()
            << " Col:" << 1 + file->cursor.x
            << "/" << file->cursor.line->size();

        if (!file->normal_mode_buffer.empty()) {
            left_oss << " | " << file->normal_mode_buffer;
        }
    }
    else
    {
        left_oss << " | no file";
    }

    std::string left_str = left_oss.str();
    return left_str.substr(0, bounds.width);
}
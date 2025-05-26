#include "Editor.h"
#include "StatusWindow.h"
#include <sstream>

void StatusWindow::draw()
{
    werase(win);
    mvwprintw(win, 0, 0, get_status().c_str());
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
    // std::ostringstream right_oss;

    File* file = editor.get_focused_file();

    if (editor.editor_mode)left_oss << editor.editor_mode->name();

    int tabno = editor.window_manager.get_tab_index() + 1;
    int ntabs = editor.window_manager.count_tabs();
    left_oss << " | tab " << tabno << " of " << ntabs;

    if (file)
    {
        if (!file->filename.empty()) {
            left_oss << " | " << file->filename;
        }

        left_oss << " | " << "Ln:" << 1+file->cursor.y
            << "/" << file->lines.size()
            << " Col:" << 1+file->cursor.x
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
    // std::string right_str = right_oss.str();

    // size_t space_count = 0;

    // if (bounds.width > left_str.size() + right_str.size()) {
    //     space_count = bounds.width - left_str.size() - right_str.size();
    //     return left_str + std::string(space_count, ' ') + right_str;
    // }
    // else {
    //     return (left_str + right_str).substr(0, bounds.width);
    // }

    return left_str.substr(0, bounds.width);
}
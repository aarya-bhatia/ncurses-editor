#include "Editor.h"
#include "StatusWindow.h"
#include <sstream>

void StatusWindow::draw()
{
    wclear(window.get());
    wprintw(window.get(), get_status().c_str());
}

void StatusWindow::show()
{
    wrefresh(window.get());
}

void StatusWindow::resize(Dimension bounds)
{
    this->bounds = bounds;
    this->window = NcursesWindow(bounds);
}

std::string StatusWindow::get_status()
{
    std::ostringstream left_oss;
    std::ostringstream right_oss;

    File* file = editor.get_focused_file();

    if (editor.editor_mode)left_oss << editor.editor_mode->name();

    int tabno = editor.window_manager.get_tab_index() + 1;
    int ntabs = editor.window_manager.count_tabs();
    left_oss << " | tab " << tabno << " of " << ntabs;

    if (file)
    {
        left_oss << " | " << file->filename << " | " << "Ln:" << file->cursor.y << " Col:" << file->cursor.x;

        if (!file->normal_mode_buffer.empty()) {
            right_oss << " | " << file->normal_mode_buffer;
        }
    }
    else
    {
        right_oss << "no file";
    }

    std::string left_str = left_oss.str();
    std::string right_str = right_oss.str();

    size_t space_count = 0;

    if (bounds.width > left_str.size() + right_str.size()) {
        space_count = bounds.width - left_str.size() - right_str.size();
        return left_str + std::string(space_count, ' ') + right_str;
    }
    else {
        return (left_str + right_str).substr(0, bounds.width);
    }
}
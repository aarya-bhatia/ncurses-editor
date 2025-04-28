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

    auto mode_name = mode_names.find(editor.mode);
    if (mode_name != mode_names.end()) {
        left_oss << mode_name->second;
    }
    if (file)
    {
        left_oss << " | " << file->filename << " | " << "Ln:" << file->cursor.y << " Col:" << file->cursor.x;

        // if (file->count_lines() > 0) {
        //     float percent_read = 100.0 * (1 + file->cursor.y) / file->count_lines();
        //     int rounded = std::round(percent_read * 10.0f) / 10.0f;
        //     right_oss << " | " << rounded << "%%";
        // }

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
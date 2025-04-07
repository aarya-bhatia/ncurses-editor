#include "application/editor.h"
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
    std::shared_ptr<File> file = editor.get_current_file();

    auto mode_name = mode_names.find(editor.mode);
    if (mode_name != mode_names.end()) {
        left_oss << "-- " << mode_name->second << " --";
    }
    if (file)
    {
        if (file->filename) {
            right_oss << file->filename;
        }
        else {
            right_oss << "Untitled";
        }

        right_oss << " | " << "Ln:" << file->cursor.y << " Col:" << file->cursor.x;
    }
    else
    {
        right_oss << "no file" << " | " << "Ln:0 Col:0";
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
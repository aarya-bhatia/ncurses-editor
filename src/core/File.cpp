#include "File.h"
#include "log.h"
#include "common.h"
#include <assert.h>
#include <string>
#include <list>

File::File(FileID _id, const std::string& _filename) :
    id(_id)
{
    filename = !_filename.empty() ? _filename : "untitled-" + std::to_string(id);

    lines.push_back({});
    cursor.x = 0;
    cursor.y = 0;
    cursor.line = this->lines.begin();
    cursor.col = this->lines.front().begin();
}

File::~File()
{
}

int File::load_file()
{
    FILE* file = fopen(filename.c_str(), "r");
    if (!file)
    {
        log_info("no lines were loaded because file %s does not exist", filename.c_str());
        return 0;
    }
    fclose(file);

    std::list<std::string> str_lines = readlines(filename.c_str());
    log_info("Read file %s with %zu lines", filename.c_str(), str_lines.size());
    lines.clear();

    for (const std::string& str_line : str_lines)
    {
        lines.emplace_back(str_line.begin(), str_line.end());
    }

    if (lines.empty())
    {
        lines.push_back({});
    }

    cursor.y = 0;
    cursor.x = 0;
    cursor.line = lines.begin();
    cursor.col = lines.front().begin();

    for (auto& subscriber : subscribers) {
        subscriber->file_changed();
    }

    return 0;
}

int File::save_file()
{
    FILE* file = fopen(filename.c_str(), "w");
    if (!file)
    {
        return 1;
    }

    for (auto line_itr = lines.begin(); line_itr != lines.end(); line_itr++)
    {
        // skip last line if it is empty.
        if (std::next(line_itr) == lines.end() && line_itr->empty()) {
            break;
        }

        for (char c : *line_itr)
        {
            fputc(c, file);
        }
        fputc('\n', file);
    }

    fclose(file);
    return 0;
}

bool File::_move_cursor_y(int dy)
{
    if (cursor.y + dy < 0 || cursor.y + dy >= lines.size()) {
        return false;
    }

    cursor.y += dy;

    if (dy > 0) {
        cursor.line = std::next(cursor.line, dy);
    }
    else {
        cursor.line = std::prev(cursor.line, -dy);
    }

    if (cursor.x >= cursor.line->size()) {
        cursor.col = cursor.line->empty() ? cursor.line->begin() : --cursor.line->end();
        cursor.x = cursor.line->empty() ? 0 : cursor.line->size() - 1;
    }
    else {
        cursor.col = cursor.line->begin();
        cursor.col = std::next(cursor.col, cursor.x);
    }

    return true;
}

bool File::_move_cursor_x(int dx)
{
    if (cursor.x + dx < 0 || cursor.x + dx >= (*cursor.line).size()) {
        return false;
    }

    cursor.x += dx;

    if (dx > 0)
    {
        cursor.col = std::next(cursor.col, dx);
    }
    else {
        cursor.col = std::prev(cursor.col, -dx);
    }

    return true;
}

void File::move_cursor_eol()
{
    cursor.col = cursor.line->empty() ? cursor.line->begin() : --cursor.line->end();
    cursor.x = cursor.line->empty() ? 0 : cursor.line->size() - 1;
}

void File::move_cursor_eof()
{
    log_info("jump to last line");
    if (count_lines() > 0) {
        goto_line(count_lines() - 1);
    }
}

void File::goto_line(int line_no)
{
    if (line_no < 0 || line_no >= count_lines()) {
        return;
    }

    _move_cursor_y(line_no - cursor.y);
}

void File::goto_column(int col)
{
    _move_cursor_y(col - cursor.x);
}

void File::cursor_up()
{
    _move_cursor_y(-1);
}

void File::cursor_down()
{
    _move_cursor_y(1);
}

void File::cursor_left()
{
    _move_cursor_x(-1);
}

void File::cursor_right()
{
    _move_cursor_x(1);
}

void File::move_begin()
{
    cursor.x = 0;
    cursor.col = cursor.line->begin();
}

void File::insert_character(int c)
{
    cursor.col = cursor.line->insert(cursor.col, c);
    cursor.col++;
    cursor.x++;

    Cursor insert_position = cursor;
    insert_position.col--;
    insert_position.x--;

    for (auto& subsciber : subscribers) {
        subsciber->character_added(insert_position);
    }
}

void File::remove_character()
{
    if (cursor.line->empty()) { return; }
    cursor.col = cursor.line->erase(cursor.col);
    if (cursor.col == cursor.line->end()) {
        cursor.col = cursor.line->empty() ? cursor.line->begin() : std::prev(cursor.col);
    }

    if (cursor.x >= cursor.line->size()) {
        cursor.x = cursor.line->empty() ? 0 : cursor.line->size() - 1;
    }

    for (auto& subscriber : subscribers) {
        subscriber->character_removed(cursor);
    }
}

void File::remove_line()
{
    assert(!lines.empty());
    cursor.line = lines.erase(cursor.line);

    if (cursor.line == lines.end() && !lines.empty()) {
        cursor.line = std::prev(lines.end());
    }

    if (lines.empty()) {
        lines.push_back({});
        cursor.line = lines.begin();
    }

    if (cursor.y >= lines.size()) {
        cursor.y--;
    }

    if (cursor.x >= cursor.line->size()) {
        cursor.x = cursor.line->empty() ? 0 : cursor.line->size() - 1;
    }

    cursor.col = cursor.line->begin();
    std::advance(cursor.col, cursor.x);

    for (auto& subscriber : subscribers) {
        subscriber->line_removed(cursor);
    }
}

void File::insert_line_below() {
    auto insert_at = std::next(cursor.line);
    cursor.line = lines.insert(insert_at, { {} });
    cursor.col = cursor.line->begin();
    cursor.y++;
    cursor.x = 0;

    for (auto& subscriber : subscribers) {
        subscriber->line_added(cursor);
    }
}

void File::paste_below(std::string& line)
{
    std::list<char> new_line(line.begin(), line.end());
    cursor.line = lines.insert(std::next(cursor.line), new_line);
    cursor.col = cursor.line->begin();
    cursor.y++;
    cursor.x = 0;

    for (auto& subscriber : subscribers) {
        subscriber->line_added(cursor);
    }
}

void File::paste_above(std::string& line)
{
    std::list<char> new_line(line.begin(), line.end());
    cursor.line = lines.insert(cursor.line, new_line);
    cursor.col = cursor.line->begin();
    cursor.x = 0;

    for (auto& subscriber : subscribers) {
        subscriber->line_added(cursor);
    }
}

std::string File::copy_line() const
{
    return std::string(cursor.line->begin(), cursor.line->end());
}

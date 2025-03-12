#include "File.h"
#include "common.h"
#include <string>
#include <list>

File::File(const char *_filename)
{
    filename = _filename;

    lines.push_back({});

    cursor.x = 0;
    cursor.y = 0;
    cursor.line = lines.begin();
    cursor.col = lines.front().begin();

    scroll.dy = 0;
    scroll.dx = 0;
}

int File::load_file()
{
    FILE *file = fopen(filename, "r");
    if(!file) {
        return 1;
    }
    fclose(file);

    std::list<std::string> str_lines = readlines(filename);
    lines.clear();

    for (const std::string &str_line : str_lines)
    {
        lines.emplace_back(str_line.begin(), str_line.end());
    }

    if (lines.empty())
    {
        lines.push_back({});
    }

    cursor.line = lines.begin();
    cursor.col = lines.front().begin();

    return 0;
}

int File::save_file()
{
    FILE *file = fopen(filename, "w");
    if(!file) {
        return 1;
    }

    for (const std::list<char> &line : lines)
    {
        for (char c : line)
        {
            fputc(c, file);
        }
        fputc('\n', file);
    }

    fclose(file);
    return 0;
}
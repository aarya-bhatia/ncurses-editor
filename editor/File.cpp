#include "File.h"
#include "log.h"
#include "common.h"
#include <string>
#include <list>

int File::load_file()
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        log_info("no lines were loaded because file %s does not exist", filename);
        return 0;
    }
    fclose(file);

    std::list<std::string> str_lines = readlines(filename);
    log_info("Read file %s with %zu lines", filename, str_lines.size());
    lines.clear();

    for (const std::string &str_line : str_lines)
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
    scroll.dx = 0;
    scroll.dy = 0;

    // subscriber->file_loaded();

    return 0;
}

int File::save_file()
{
    FILE *file = fopen(filename, "w");
    if (!file)
    {
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
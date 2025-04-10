#include "common.h"
#include <list>
#include <string>
#include <vector>
#include <assert.h>

int char_type(char c)
{
    if (isalnum(c))
    {
        return C_ALPHANUM;
    }
    else if (isspace(c))
    {
        return C_WHITESPACE;
    }
    else if (c > 0 && c < 32)
    {
        return C_CONTROL;
    }
    else if ((c > ' ' && c < '0') || (c > 'Z' && c < 'a') || (c > 'z'))
    {
        return C_SPECIAL;
    }

    return C_UNDEFINED;
}

std::list<std::string> readlines(const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (!file)
    {
        perror("fopen");
        return {};
    }

    std::list<std::string> ans;
    char* line = NULL;
    size_t len = 0;
    ssize_t nread;
    while ((nread = getline(&line, &len, file)) > 0)
    {
        if (line[nread - 1] == '\n')
        {
            line[nread - 1] = 0;
        }
        ans.emplace_back(line);
    }
    free(line);
    fclose(file);
    return ans;
}

// Get list of non-empty tokens from 'line' separated by the exact substring of 'delim'.
std::vector<std::string> splitwords(const std::string& line, const std::string& delim)
{
    std::vector<std::string> tokens;
    size_t start = 0, end;

    while ((end = line.find(delim, start)) != std::string::npos)
    {
        if (end > start)
        { // Avoid empty tokens (e.g., consecutive delimiters)
            tokens.emplace_back(line.substr(start, end - start));
        }
        start = end + delim.length();
    }

    if (start < line.size())
    { // Add the last token
        tokens.emplace_back(line.substr(start));
    }

    return tokens;
}

bool is_number(const std::string& str) {
    return !str.empty() &&
        std::all_of(str.begin(), str.end(), ::isdigit);
}

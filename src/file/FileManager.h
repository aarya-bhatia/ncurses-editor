#pragma once
#include "File.h"

class FileManager
{
    std::vector<File*> _files;
    size_t _index = 0;

public:
    FileManager() = default;

    File* get_file();

    File* open_untitled_file();

    int open_file(const char* filename);
    int close_file();
    int next_file();
    int prev_file();

    bool has_file(const char* filename);

    size_t count_files() const { return _files.size(); }
    size_t get_file_index() const { return _index; }

    FileID get_new_file_id() const;
};
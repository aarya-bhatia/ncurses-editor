#pragma once
#include <memory>
#include <map>
#include "File.h"
#include "FileView.h"

class FileManager
{
    std::vector<std::shared_ptr<File>> _files;
    std::map<std::shared_ptr<File>, ContentWindow*> _file_views;

public:
    FileManager() = default;

    std::shared_ptr<File> get_file(const char* filename);
    ContentWindow* get_file_view(const std::shared_ptr<File>& file);

    std::shared_ptr<File> open_untitled_file();
    std::shared_ptr<File> open_file(const char* filename);
    void close_file(const std::shared_ptr<File>& file);
    std::shared_ptr<File> next_file(const std::shared_ptr<File>& file);
    std::shared_ptr<File> prev_file(const std::shared_ptr<File>& file);
    FileID get_new_file_id() const;
    size_t count_files() const { return _files.size(); }
};
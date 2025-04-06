#pragma once
#include <memory>
#include <map>
#include "File.h"
#include "FileView.h"
#include "window/IWindowManager.h"

class FileManager
{
    std::vector<std::shared_ptr<File>> _files;
    std::shared_ptr<IWindowManager> window_manager;

public:
    FileManager(std::shared_ptr<IWindowManager> wm) : window_manager(std::move(wm)) {
    }

    std::shared_ptr<File> get_file(const char* filename);

    std::shared_ptr<File> open_untitled_file();
    std::shared_ptr<File> open_file(const char* filename);
    void close_file(const std::shared_ptr<File>& file);
    std::shared_ptr<File> next_file(const std::shared_ptr<File>& file);
    std::shared_ptr<File> prev_file(const std::shared_ptr<File>& file);
    FileID get_new_file_id() const;
    size_t count_files() const { return _files.size(); }

    void open_in_current_window(std::shared_ptr<File> file);
    void open_in_splith(std::shared_ptr<File> file);
    void open_in_splitv(std::shared_ptr<File> file);
};
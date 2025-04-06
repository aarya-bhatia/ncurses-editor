#include "FileManager.h"
#include "log.h"
#include <assert.h>

ContentWindow* FileManager::get_file_view(const std::shared_ptr<File>& file)
{
    if (!file) {
        return nullptr;
    }

    if (_file_views.find(file) == _file_views.end()) {
        _file_views[file] = new FileView(file);
    }

    return _file_views[file];
}

std::shared_ptr<File> FileManager::get_file(const char* filename)
{
    for (auto& file : _files) {
        if (!strncmp(file->filename, filename, strlen(filename))) {
            return file;
        }
    }

    return nullptr;
}

std::shared_ptr<File> FileManager::open_file(const char* filename)
{
    log_debug("Opening file: %s", filename);
    auto file = get_file(filename);
    if (file) {
        return file;
    }

    FileID id = get_new_file_id();
    std::shared_ptr<File> new_file = std::shared_ptr<File>(new File(id, filename));
    if (new_file->load_file() != 0)
    {
        log_warn("Failed to load file %s", filename);
        return nullptr;
    }

    _files.push_back(new_file);
    log_info("Added file: %s", filename);
    return new_file;
}

void FileManager::close_file(const std::shared_ptr<File>& file)
{
    _files.erase(std::find(_files.begin(), _files.end(), file));
    _file_views.erase(file);
}

std::shared_ptr<File> FileManager::next_file(const std::shared_ptr<File>& file)
{
    for (size_t i = 0; i < _files.size(); i++) {
        if (_files[i] == file) {
            if (i == _files.size() - 1) {
                return _files[0];
            }
            else {
                return _files[i + 1];
            }
        }
    }

    return file;
}

std::shared_ptr<File> FileManager::prev_file(const std::shared_ptr<File>& file)
{
    for (size_t i = 0; i < _files.size(); i++) {
        if (_files[i] == file) {
            if (i == 0) {
                return _files[_files.size() - 1];
            }
            else {
                return _files[i - 1];
            }
        }
    }

    return file;
}

FileID FileManager::get_new_file_id() const
{
    size_t id = 0;

    // find next available id from file list
    for (size_t i = 0; i < _files.size(); i++)
    {
        if (_files[i]->id > id)
        {
            id = _files[i]->id;
        }
    }

    return id + 1;
}

std::shared_ptr<File> FileManager::open_untitled_file()
{
    size_t id = get_new_file_id();
    std::shared_ptr<File> new_file = std::shared_ptr<File>(new File(id, nullptr));
    _files.push_back(new_file);
    return new_file;
}

void FileManager::open_in_splith(std::shared_ptr<File>& file) {
    ContentWindow* content = new FileView(file, window_manager->get_bounds());
    window_manager->split_horizontal(content);
}

void FileManager::open_in_splitv(std::shared_ptr<File>& file) {
    ContentWindow* content = new FileView(file, window_manager->get_bounds());
    window_manager->split_vertical(content);
}

void FileManager::open_in_current_window(std::shared_ptr<File>& file)
{
    ContentWindow* content = new FileView(file, window_manager->get_bounds());
    window_manager->set_content(content);
}
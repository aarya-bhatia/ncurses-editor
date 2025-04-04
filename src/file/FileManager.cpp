#include "FileManager.h"
#include "log.h"

File* FileManager::get_file()
{
    if (_files.empty())
    {
        return NULL;
    }

    return _files[_index];
}

int FileManager::open_file(const char* filename)
{
    log_debug("Opening file: %s", filename);
    for (size_t i = 0; i < _files.size(); i++)
    {
        if (!strncmp(_files[i]->filename, filename, strlen(filename)))
        {
            log_info("opened file %s at index %d", filename, _index);
            _index = i;
            return 0;
        }
    }

    FileID id = get_new_file_id();
    File* new_file = new File(id, filename);
    if (new_file->load_file() != 0)
    {
        log_warn("Failed to load file %s", filename);
        delete new_file;
        return 1;
    }

    _files.push_back(new_file);
    _index = _files.size() - 1;
    log_info("New file %s added at index %d", filename, _index);

    return 0;
}

int FileManager::close_file()
{
    if (_files.empty())
    {
        return 1;
    }

    delete _files[_index];
    _files.erase(_files.begin() + _index);
    _index = 0;
    return 0;
}

int FileManager::next_file()
{
    if (_files.empty())
    {
        return 1;
    }

    if (_index >= _files.size() - 1)
    {
        _index = 0;
    }
    else
    {
        _index++;
    }

    return 0;
}

int FileManager::prev_file()
{
    if (_files.empty())
    {
        return 1;
    }

    if (_index == 0)
    {
        _index = _files.size() - 1;
    }
    else
    {
        _index--;
    }

    return 0;
}

bool FileManager::has_file(const char* filename)
{
    for (size_t i = 0; i < _files.size(); i++)
    {
        if (!strncmp(_files[i]->filename, filename, strlen(filename)))
        {
            return true;
        }
    }

    return false;
}

FileID FileManager::get_new_file_id() const
{
    size_t id = 0;
    for (size_t i = 0; i < _files.size(); i++)
    {
        if (_files[i]->id > id)
        {
            id = _files[i]->id;
        }
    }

    return id + 1;
}

File* FileManager::open_untitled_file()
{
    size_t id = get_new_file_id();
    File* new_file = new File(id, nullptr);
    _files.push_back(new_file);
    _index = _files.size() - 1;
    return new_file;
}
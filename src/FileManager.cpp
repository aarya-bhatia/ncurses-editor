#include "FileManager.h"
#include "log.h"

File *FileManager::get_file()
{
    if(_files.empty()) {
        return NULL;
    }

    return _files[_index];
}

int FileManager::open_file(const char *filename)
{
    log_debug("Opening file: %s", filename);
    for(size_t i = 0; i < _files.size(); i++)
    {
        if(!strncmp(_files[i]->filename, filename, strlen(filename))) {
            log_info("opened file %s at index %d", filename, _index);
            _index = i;
            return 0;
        }
    }

    File *new_file = new File(filename);
    if(new_file->load_file() != 0) {
        log_warn("Failed to load file %s", filename);
        delete new_file;
        return 1;
    }

    log_info("New file %s added at index %d", filename, _index);
    _files.push_back(new_file);
    _index = _files.size() - 1;
    return 0;
}

int FileManager::close_file()
{
    if(_files.empty()) {
        return 1;
    }

    delete _files[_index];
    _files.erase(_files.begin() + _index);
    _index = 0;
    return 0;
}

int FileManager::next_file()
{
    if(_files.empty()) {
        return 1;
    }

    _index++;
    if(_index >= _files.size()) {
        _index = 0;
    }

    return 0;
}

int FileManager::prev_file()
{
    if(_files.empty()) {
        return 1;
    }

    _index--;
    if(_index < 0) {
        _index = _files.size() - 1;
    }

    return 0;
}

bool FileManager::has_file(const char *filename)
{
    for(size_t i = 0; i < _files.size(); i++)
    {
        if(!strncmp(_files[i]->filename, filename, strlen(filename))) {
            return true;
        }
    }

    return false;
}
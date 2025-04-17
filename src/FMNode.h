#pragma once
#include "File.h"

struct FMNode
{
    File* file = nullptr;

    FMNode(int file_id, const char* filename) {
        file = new File(file_id, filename);
        file->load_file();
    }

    ~FMNode()
    {
        delete file;
    }

    bool has_file(const char* filename) const {
        return file->filename && !strncmp(file->filename, filename, strlen(filename));
    }
};

#pragma once
#include <vector>
#include "File.h"
#include "FileView.h"

struct FMNode
{
    File* file = nullptr;
    std::vector<FileView*> views;

    FMNode(int file_id, const char* filename) {
        file = new File(file_id, filename);
        file->load_file();
    }

    ~FMNode()
    {
        for (FileView* view : views) {
            delete view;
        }

        delete file;
    }

    FileView* add_view(Dimension d) {
        FileView* view = new FileView(file, d);
        views.push_back(view);
        return view;
    }

    bool has_file(const char* filename) const {
        return file->filename && !strncmp(file->filename, filename, strlen(filename));
    }
};

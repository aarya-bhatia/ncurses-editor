#pragma once
#include <vector>
#include "File.h"
#include "FileView.h"
#include "SequenceGenerator.h"

struct FMNode
{
    File* file = nullptr;
    std::vector<FileView*> views;

    FMNode(int file_id, const char* filename) {
        file = new File(file_id, filename);
    }

    ~FMNode()
    {
        for (FileView* view : views) {
            delete view;
        }

        delete file;
    }

    FileView* add_view(Dimension d) {
        views.push_back(new FileView(file, d));
        return views.back();
    }

    bool has_file(const char* filename) const {
        return file->filename && !strncmp(file->filename, filename, strlen(filename));
    }
};

class FileManager
{
    LinearSequenceGenerator<int> id_gen;
    std::list<FMNode*> nodes;

public:
    FileManager();
    ~FileManager();

    FMNode* find(const char* filename);
    FMNode* add_file(const char* filename);
};
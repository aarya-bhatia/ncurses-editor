#pragma once
#include "FMNode.h"
#include "SequenceGenerator.h"
#include "FileUpdateHandler.h"
#include <list>

class FileManager
{
    FileUpdateHandler& file_update_handler;
    LinearSequenceGenerator<int> id_gen;
    std::list<FMNode*> nodes;

public:
    FileManager(FileUpdateHandler& handler) : file_update_handler(handler), id_gen(1) {
    }

    ~FileManager()
    {
        for (auto* node : nodes)
        {
            delete node;
        }
    }

    FMNode* find(const char* filename)
    {

        for (auto* node : nodes) {
            if (node->has_file(filename)) return node;
        }

        return nullptr;
    }

    FMNode* add_file(const char* filename) {
        FMNode* node = find(filename);
        if (node) {
            return node;
        }

        node = new FMNode(id_gen.next(), filename);
        nodes.push_back(node);
        node->file->add_subscriber(&file_update_handler);
        return node;
    }
};
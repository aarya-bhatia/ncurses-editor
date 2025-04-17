#pragma once
#include "FMNode.h"
#include "SequenceGenerator.h"
#include <list>

class FileManager
{
    LinearSequenceGenerator<int> id_gen;
    std::list<FMNode*> nodes;

public:
    FileManager()
        :id_gen(1) {
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
        return node;
    }
};
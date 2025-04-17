#include "FileManager.h"
#include "util/log.h"
#include <assert.h>

FileManager::FileManager() : id_gen(1) {
}

FileManager::~FileManager()
{
    for (auto* node : nodes)
    {
        delete node;
    }
}

FMNode* FileManager::find(const char* filename)
{
    for (auto* node : nodes) {
        if (node->has_file(filename)) return node;
    }

    return nullptr;
}

FMNode* FileManager::add_file(const char* filename)
{
    FMNode* node = find(filename);
    if (node) {
        return node;
    }

    node = new FMNode(id_gen.next(), filename);
    nodes.push_back(node);
    return node;
}
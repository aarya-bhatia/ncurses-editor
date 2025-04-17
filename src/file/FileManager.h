#pragma once
#include "FMNode.h"
#include "SequenceGenerator.h"
#include <list>

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
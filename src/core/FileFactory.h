#pragma once

#include "File.h"
#include "SequenceGenerator.h"

struct FileFactory
{
    static File* new_file(const std::string &filename = "") {
        static LinearSequenceGenerator<int> id_gen(1);
        return new File(id_gen.next(), filename);
    }
};
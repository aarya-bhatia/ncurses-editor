#pragma once
#include "BorderedFileView.h"

struct ViewFactory {
    static Window *new_file_view(File *file, Dimension d){
        return new BorderedFileView(file, d);
    }
};
#pragma once
#include "BorderedFileView.h"

struct ViewFactory {
    Window *new_file_view(File *file, Dimension d){
        return new BorderedFileView(new FileView(file, d));
    }
};
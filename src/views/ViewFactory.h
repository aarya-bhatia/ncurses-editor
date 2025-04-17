#pragma once
#include "BorderedFileView.h"

struct ViewFactory {
    static Window *new_file_view(File *file, Dimension d){
        Window *window = new BorderedFileView(file, d);
        // file->add_subscriber(window);
        return window;
    }
};
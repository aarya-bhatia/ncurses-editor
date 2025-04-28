#pragma once
#include "ViewContainer.h"

struct ViewFactory {
    static Window* new_file_view(File* file, Dimension d) {
        ViewContainer* view = new ViewContainer(d);
        if (file) {
            view->set_file(file);
            FileView* file_view = new FileView(file, d);
            view->open_view(file_view);
        }
        return view;
    }
};
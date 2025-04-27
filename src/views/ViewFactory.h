#pragma once
// #include "ViewBuilder.h"
#include "ViewContainer.h"

struct ViewFactory {
    static Window* new_file_view(File* file, Dimension d) {
        // return ViewBuilder(file, d).add_border().build();
        ViewContainer* view = new ViewContainer(d);
        if (file)view->set_file(file);
        return view;
    }
};
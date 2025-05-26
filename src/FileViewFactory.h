#pragma once

#include "FileView.h"
#include "ViewContainer.h"

struct FileViewFactory {
    static Window* create_content_window(Dimension d, File* file = nullptr) {
        return new ViewContainer(d, file);
    }

    static FileView* get_file_view(Window* content_window) {
        if (!content_window) {
            return nullptr;
        }
        ViewContainer* container = dynamic_cast<ViewContainer*>(content_window);
        if (!container) {
            return nullptr;
        }
        return container->get_file_view();
    }
};
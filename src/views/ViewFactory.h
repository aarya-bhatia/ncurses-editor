#pragma once
#include "ViewBuilder.h"

struct ViewFactory {
    static Window* new_file_view(File* file, Dimension d) {
        return ViewBuilder(file, d).add_border().build();
    }
};
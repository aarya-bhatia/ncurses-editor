#pragma once
#include "Window.h"
#include "FileView.h"
#include "BorderedFileView.h"

struct ViewBuilder
{
    Window* current = nullptr;

    ViewBuilder(File* file, Dimension d) {
        current = new FileView(file, d);
    }

    ViewBuilder& add_border() {
        current = new BorderedFileView(current->get_file(), current->get_bounds());
        return *this;
    }

    Window* build() {
        return current;
    }
};
#pragma once
#include <ncurses.h>
#include <memory>
#include "window/Dimension.h"

struct WindowDeleter {
    void operator()(WINDOW* window) const {
        if (window) {
            delwin(window);
        }
    }
};

struct NcursesWindow {
    std::unique_ptr<WINDOW, WindowDeleter> window;

    NcursesWindow() {
    }

    NcursesWindow(Dimension dim) {
        window = std::unique_ptr<WINDOW, WindowDeleter>(
            newwin(dim.height, dim.width, dim.y, dim.x)
        );
    }

    WINDOW* get() const noexcept { return window.get(); }
};


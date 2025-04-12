#include "Window.h"
#include "ContainerWindow.h"
#include "ContentWindow.h"
#include <assert.h>
#include "log.h"
#include <string>

bool Window::resizable(Dimension bounds) {
    if (get_container()) {
        ContainerWindow* container = get_container();
        if (!container->resize_strategy->is_executable(container->children, container->bounds, bounds)) {
            return false;
        }
    }

    return bounds.width >= MIN_WINDOW_SIZE && bounds.height >= MIN_WINDOW_SIZE;
}

void Window::resize(Dimension bounds) {
    if (get_container()) log_debug("resizing container window to %d lines x %d cols", bounds.height, bounds.width);
    else log_debug("resizing content window to %d lines x %d cols", bounds.height, bounds.width);
    assert(resizable(bounds));

    if (get_container()) {
        ContainerWindow* container = get_container();
        container->resize_strategy->execute(container->children, container->bounds, bounds);
    }

    this->bounds = bounds;
}

std::string Window::debug_string()const {
    return "window-" + std::to_string(id) + "+" + bounds.debug_string();
}
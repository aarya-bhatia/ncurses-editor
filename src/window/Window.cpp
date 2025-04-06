#include "Window.h"
#include "ContainerWindow.h"
#include "ContentWindow.h"

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
    if (!resizable(bounds)) {
        return;
    }

    if (get_container()) {
        ContainerWindow* container = get_container();
        container->resize_strategy->execute(container->children, container->bounds, bounds);
    }

    this->bounds = bounds;
}
#include "ContainerWindow.h"

ContainerWindow::ContainerWindow(Dimension bounds, WindowResizeStrategy* resize_strategy) :
    Window(bounds), resize_strategy(resize_strategy) {
    if (!resize_strategy) {
        resize_strategy = new ProportionalResizeStrategy;
    }
}

ContainerWindow::~ContainerWindow() {
    delete resize_strategy;
    resize_strategy = nullptr;

    for (auto child : children) {
        delete child;
    }

    children.clear();
}

size_t ContainerWindow::count_children() const {
    return children.size();
}

bool ContainerWindow::has_child(Window* child) const {
    return std::find(children.begin(), children.end(), child) != children.end();
}

void ContainerWindow::swap_child(Window* old_child, Window* new_child) {
    for (size_t i = 0; i < children.size(); i++) {
        if (children[i] == old_child) {
            old_child->parent = nullptr;
            children[i] = new_child;
            new_child->parent = this;
            return;
        }
    }
}

void ContainerWindow::add_child(Window* child) {
    children.push_back(child);
    child->parent = this;
}

void ContainerWindow::remove_child(Window* child) {
    children.erase(std::remove(children.begin(), children.end(), child), children.end());
    child->parent = nullptr;
}

void ContainerWindow::arrange_children() {
    resize_strategy->execute(children, bounds, bounds);
}

ContainerWindow* ContainerWindow::get_container()  { return this; }

void ContainerWindow::draw()  {
    for (auto child : children) {
        child->draw();
    }
}

void ContainerWindow::show()  {
    for (auto child : children) {
        child->show();
    }
}

bool ContainerWindow::resize(Dimension new_bounds)  {
    Window::resize(bounds);

    if (resize_strategy->execute(children, bounds, bounds)) {
        this->bounds = new_bounds;
        return true;
    }

    return false;
}

#include "ContainerWindow.h"
#include <assert.h>

ContainerWindow::ContainerWindow(Dimension bounds, WindowResizeStrategy* resize_strategy) :
    Window(bounds), resize_strategy(resize_strategy) {
    assert(resize_strategy);
}

ContainerWindow::~ContainerWindow() {
    delete resize_strategy;

    for (auto child : children) {
        delete child;
    }

    children.clear();
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

void ContainerWindow::draw() {
    for (auto child : children) {
        child->draw();
    }
}

void ContainerWindow::show() {
    for (auto child : children) {
        child->show();
    }
}

void ContainerWindow::detach_from_layout()
{
    if (this->parent) {
        this->parent->remove_child(this);
        this->parent = nullptr;
    }
}

void ContainerWindow::adopt_child(Window* child)
{
    detach_from_layout();
    this->parent = child->parent;
    if (this->parent) this->parent->swap_child(child, this);
    this->add_child(child);
    child->parent = this;
    // this->arrange_children();
}
#pragma once
#include "Window.h"

struct ContentWindow : public Window
{
    ContentWindow(Dimension bounds) : Window(bounds) {}
    ContentWindow(const ContentWindow& other) : ContentWindow(other.bounds) {
    }

    virtual ~ContentWindow() = default;
    ContentWindow* get_content() override { return this; }
};
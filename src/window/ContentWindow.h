#pragma once
#include "Window.h"
#include <memory>

struct ContentWindow : public Window
{
    enum ContentType { TestContent, FileContent };

    ContentWindow(Dimension bounds) : Window(bounds) {}
    ContentWindow(const ContentWindow& other) : ContentWindow(other.bounds) {
    }

    virtual ~ContentWindow() = default;
    ContentWindow* get_content() override { return this; }
    virtual std::shared_ptr<void> get_model() = 0;
    virtual ContentType get_content_type() = 0;
};
#pragma once
#include "Window.h"
#include <memory>
#include <string>
#include "log.h"

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

    virtual void on_focus() override {
        log_info("Got focus on %s", debug_string().c_str());
    }

    virtual void on_unfocus() override {
        log_info("Lost focus from %s", debug_string().c_str());
    }

    std::string debug_string() {
        return "[content window at " + bounds.debug_string() + "]";
    }

    Window* get_right_most_content_node() override { return this; }
    Window* get_left_most_content_node() override { return this; }
    Window* get_top_most_content_node() override { return this; }
    Window* get_bottom_most_content_node() override { return this; }
};
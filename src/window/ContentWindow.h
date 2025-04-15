#pragma once
#include "Window.h"
#include <memory>
#include <string>
#include "util/log.h"

struct ContentWindow : public Window
{
    enum ContentType { NilContent, TestContent, FileContent };

    ContentWindow(Dimension bounds) : Window(bounds) {}
    ContentWindow(const ContentWindow& other) : ContentWindow(other.bounds) {
    }

    virtual ~ContentWindow() = default;
    ContentWindow* get_content() override { return this; }
    virtual std::shared_ptr<void> get_model() { return nullptr; }
    virtual ContentType get_content_type() { return NilContent; }

    virtual void on_focus() override {
        log_info("Got focus on %s", debug_string().c_str());
    }

    virtual void on_unfocus() override {
        log_info("Lost focus from %s", debug_string().c_str());
    }

    std::string debug_string() const override {
        return "content-" + Window::debug_string();
    }

    ContentWindow* get_right_most_content_node() override { return this; }
    ContentWindow* get_left_most_content_node() override { return this; }
    ContentWindow* get_top_most_content_node() override { return this; }
    ContentWindow* get_bottom_most_content_node() override { return this; }
};
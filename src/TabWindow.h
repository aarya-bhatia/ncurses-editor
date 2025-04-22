#pragma once

#include "Window.h"
#include "BorderedFileView.h"

struct TabWindow : public Window
{
    using Tabs = std::list<Window*>;
    using Tab = Tabs::iterator;

    Tabs tabs;
    Tab current_tab;

    File* empty_file;
    Window* empty_window;

    Dimension bounds;

    TabWindow(Dimension d)
    {
        bounds = d;
        empty_file = new File(0, nullptr);
        empty_window = new BorderedFileView(empty_file, d);
        current_tab = tabs.begin();
    }

    ~TabWindow()
    {
        delete empty_window;
        delete empty_file;

        for (Window* window : tabs) {
            delete window;
        }
    }

    void add_tab(Window* child_window) {
        if (!child_window) {
            return;
        }

        tabs.push_back(child_window);

        if (current_tab == tabs.end()) {
            current_tab = tabs.begin();
            focus();
        }
    }

    size_t count_tabs() const { return tabs.size(); }

    bool has_next_tab() {
        return !empty() && std::next(current_tab) != tabs.end();
    }

    bool has_prev_tab() {
        return !empty() && current_tab != tabs.begin();
    }

    bool has_other_tabs() {
        return tabs.size() > 1;
    }

    bool empty() {
        return tabs.empty();
    }

    void close_all() {
        unfocus();
        tabs.clear();
        current_tab = tabs.begin();
        focus();
    }

    void open_first() {
        if (empty()) { return; }
        unfocus();
        current_tab = tabs.begin();
        focus();
    }

    void open_last() {
        if (empty()) { return; }
        unfocus();
        current_tab = std::prev(tabs.end());
        focus();
    }

    void open_next() {
        if (!has_next_tab()) { return; }

        unfocus();
        current_tab = std::next(current_tab);
        focus();
    }

    void open_prev() {
        if (!has_prev_tab()) { return; }
        unfocus();
        current_tab = std::prev(current_tab);
        focus();
    }

    void close_current_tab() {
        unfocus();

        if (!has_other_tabs()) {
            tabs.erase(current_tab);
            current_tab = tabs.begin();
        }
        else if (std::next(current_tab) == tabs.end()) {
            Tab new_tab = std::prev(current_tab);
            tabs.erase(current_tab);
            current_tab = new_tab;
        }
        else {
            Tab new_tab = std::next(current_tab);
            tabs.erase(current_tab);
            current_tab = new_tab;
        }

        focus();
    }

    Window* current_window()
    {
        if (current_tab == tabs.end() || *current_tab == nullptr) {
            return empty_window;
        }

        if (!empty()) {
            current_tab = tabs.begin();
        }

        return *current_tab;
    }

    File* get_file() {
        return current_window()->get_file();
    }

    Window* copy(Dimension d) {
        return new TabWindow(bounds);
    }

    void clear() {
        current_window()->clear();
    }

    Dimension get_bounds() {
        return bounds;
    }

    void focus() {
        if (current_window()->get_bounds() != bounds) {
            current_window()->resize(bounds);
        }
        current_window()->focus();
    }

    void unfocus() {
        current_window()->unfocus();
    }

    void draw() {
        current_window()->draw();
    }

    void show() {
        current_window()->show();
    }

    void resize(Dimension d) {
        current_window()->resize(d);
        bounds = d;
    }

    void partial_draw_character(Cursor position) {
        if (current_window() == empty_window) {
            return;
        }

        current_window()->partial_draw_character(position);
    }

    void partial_draw_line(Cursor position) {
        if (current_window() == empty_window) {
            return;
        }

        current_window()->partial_draw_line(position);
    }

    void force_redraw() {
        current_window()->force_redraw();
    }
};
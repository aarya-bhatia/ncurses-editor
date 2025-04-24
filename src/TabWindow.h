#pragma once

#include "Window.h"
#include "BorderedFileView.h"
#include <assert.h>

struct TabNode
{
    TabNode* prev = nullptr;
    TabNode* next = nullptr;
    Window* window = nullptr;

    TabNode(TabNode* _prev, TabNode* _next, Window* w) {
        prev = _prev;
        next = _next;
        window = w;

        assert_ok();
    }

    void assert_ok() {
        assert(prev->next == this);
        assert(next->prev == this);
        assert(this->next == next);
        assert(this->prev == prev);
        assert(window);
    }

    ~TabNode() {
        delete window;
    }

    void close() {
        if (prev) {
            prev->next = next;
        }
        if (next) {
            next->prev = prev;
        }
        prev = next = nullptr;
    }

    void insert_after(TabNode* other) {
        other->next = next;
        if (next) {
            next->prev = other;
        }
        next = other;
        other->prev = this;
    }

    void insert_before(TabNode* other) {
        other->prev = prev;
        if (prev) {
            prev->next = other;
        }

        other->next = this;
        this->prev = other;
    }

    int count_forward() const {
        int c = 0;
        for (auto node = this; node; node = node->next) {
            c += 1;
        }
        return c;
    }

    int count_backward() const {
        int c = 0;
        for (auto node = this; node; node = node->prev) {
            c += 1;
        }
        return c;
    }

    void remove_after() {
        if (!next) {
            return;
        }

        TabNode* tmp = next;
        next = tmp->next;
        if (next) next->prev = this;
        delete tmp;
    }

    void remove_before() {
        if (!prev) {
            return;
        }

        TabNode* tmp = prev;
        prev = tmp->prev;
        if (prev) prev->next = this;
        delete tmp;
    }

    void remove_all_after()
    {
        TabNode* itr = next;
        while (itr) {
            TabNode* tmp = itr->next;
            delete itr;
            itr = tmp;
        }
        next = nullptr;
    }

    void remove_all_before()
    {
        TabNode* itr = prev;
        while (itr) {
            TabNode* tmp = itr->prev;
            delete itr;
            itr = tmp;
        }
        prev = nullptr;
    }

};

struct TabWindow
{
    TabNode* tabs = nullptr;
    TabNode* focused_tab = nullptr;
    Dimension bounds;

    File* empty_file = new File;

    TabWindow(Dimension d)
    {
        bounds = d;
        init();
    }

    ~TabWindow()
    {
        TabNode* tmp = nullptr;
        for (TabNode* node = tabs; node; node = tmp) {
            tmp = node->next;
            delete node;
        }

        delete tabs;
        delete empty_file;
    }

    void insert(TabNode* new_tab) {
        assert(new_tab);
        assert(new_tab->window);

        tabs->insert_before(new_tab);
        tabs = new_tab;
    }

    TabNode* find(Window* window)
    {
        for (TabNode* node = tabs; node; node = node->next) {
            if (node->window == window) {
                return node;
            }
        }

        return nullptr;
    }

    void open(Window* window) {
        TabNode* new_tab = find(window);

        if (new_tab == focused_tab) {
            return;
        }

        if (!new_tab) {
            new_tab = new TabNode(nullptr, nullptr, window);
            insert(new_tab);
        }

        focused_tab->window->unfocus();
        new_tab->window->focus();
        focused_tab = new_tab;
    }

    size_t count_tabs() const { return tabs->count_forward(); }

    void init()
    {
        Window* empty_window = new BorderedFileView(empty_file, bounds);
        tabs = new TabNode(nullptr, nullptr, empty_window);
        focused_tab = tabs;
    }

    void close_all() {
        tabs->remove_all_after();
        tabs = nullptr;
        init();
    }

    void open_first() {
        if (focused_tab == tabs)return;
        focused_tab->window->unfocus();
        focused_tab = tabs;
        focused_tab->window->focus();
    }

    void open_last() {
        if (!focused_tab->next)return;
        focused_tab->window->unfocus();
        focused_tab = tabs->last();
        focused_tab->window->focus();
    }

    void open_next() {
        if (!focused_tab->next)return;
        focused_tab->window->unfocus();
        focused_tab = tabs->next;
        focused_tab->window->focus();
    }

    void open_prev() {
        if (!focused_tab->prev)return;
        focused_tab->window->unfocus();
        focused_tab = tabs->prev;
        focused_tab->window->focus();
    }

    void close_current_tab() {
        if (focused_tab->prev) {
            auto tmp = focused_tab->prev;
            tmp->remove_after();
            focused_tab = tmp;
            tmp->window->focus();
        }
        else if (focused_tab->next) {
            auto tmp = focused_tab->next;
            tmp->remove_before();
            focused_tab = tmp;
            tmp->window->focus();
        }
        else {
            delete focused_tab;
            focused_tab = nullptr;
            init();
        }
    }

    Window* get_focused_window()
    {
        return focused_tab->window;
    }

    void clear() {
        get_focused_window()->clear();
    }

    Dimension get_bounds() {
        return bounds;
    }

    void focus() {
        // if (get_focused_window()->get_bounds() != bounds) {
        //     get_focused_window()->resize(bounds);
        // }
        get_focused_window()->focus();
    }

    void unfocus() {
        get_focused_window()->unfocus();
    }

    void draw() {
        get_focused_window()->draw();
    }

    void show() {
        get_focused_window()->show();
    }

    void resize(Dimension d) {
        for (auto* node = tabs; node; node = node->next) {
            node->window->resize(d);
        }
        // get_focused_window()->resize(d);
        bounds = d;
    }
};
#pragma once

#include "Window.h"
#include "BorderedFileView.h"
#include <assert.h>
#include "ListNode.h"

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
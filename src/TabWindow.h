#pragma once

#include "List.h"
#include "Window.h"
#include "BorderedFileView.h"
#include <assert.h>

struct TabWindow : IDrawable, IFocusable
{
    List<Window*> tabs;
    ListNode<Window*>* focused_tab = nullptr;
    Dimension bounds;

    File empty_file;

    TabWindow(Dimension d)
    {
        bounds = d;
        init();
    }

    ~TabWindow()
    {
        for (ListNode<Window*>* itr = tabs.head; itr; itr = itr->next) {
            delete itr->data;
            itr->data = nullptr;
        }
    }

    ListNode<Window*>* find_tab_by_file(File* file) {
        for (ListNode<Window*>* itr = tabs.head; itr; itr = itr->next) {
            if (itr->data->get_file() == file) {
                return itr;
            }
        }

        return nullptr;
    }

    void open(Window* window) {
        tabs.insert_back(window);
        assert(tabs.tail);
        assert(tabs.tail->data == window);
        open(tabs.tail);
    }

    void open(ListNode<Window*>* tab) {
        if (!tab) return;
        if (focused_tab == tab) return;
        if (focused_tab) focused_tab->data->unfocus();
        focused_tab = tab;
        focused_tab->data->focus();
    }

    void init()
    {
        Window* empty_window = new BorderedFileView(&empty_file, bounds);
        open(empty_window);
    }

    void close_all() {
        focused_tab = nullptr;
        for (ListNode<Window*>* itr = tabs.head; itr; itr = itr->next) {
            delete itr->data;
            itr->data = nullptr;
        }
        tabs.remove_all();
        init();
    }

    void open_first() {
        open(tabs.head);
    }

    void open_last() {
        open(tabs.tail);
    }

    void open_next() {
        if (focused_tab) open(focused_tab->next);
    }

    void open_prev() {
        if (focused_tab) open(focused_tab->prev);
    }

    void close_tab() {
        if (!focused_tab) {
            return;
        }

        ListNode<Window*>* new_tab = focused_tab->next;
        if (!new_tab) {
            new_tab = focused_tab->prev;
        }

        delete focused_tab->data;
        tabs.remove(focused_tab);

        focused_tab = new_tab;

        if (!new_tab) {
            init();
        }
    }

    Window* get_focused_window()
    {
        return focused_tab->data;
    }

    Dimension get_bounds() {
        return bounds;
    }

    void focus() {
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

    void clear() {
        get_focused_window()->clear();
    }

    void resize(Dimension d) {
        bounds = d;
        for (ListNode<Window*>* node = tabs.head; node; node = node->next) {
            node->data->resize(d);
        }
    }
};
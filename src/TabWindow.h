#pragma once

#include "List.h"
#include "Window.h"
#include "BorderedFileView.h"
#include <assert.h>

template<typename T>
struct TabWindow
{
    List<T> tabs;
    ListNode<T>* focused_tab = nullptr;
    Dimension bounds;

    TabWindow(Dimension d)
    {
        bounds = d;
    }

    ~TabWindow()
    {
        for (ListNode<T>* itr = tabs.head; itr; itr = itr->next) {
            delete itr->data;
            itr->data = nullptr;
        }
    }

    void open(T window) {
        tabs.insert_back(window);
        assert(tabs.tail);
        assert(tabs.tail->data == window);
        open(tabs.tail);
    }

    void open(ListNode<T>* tab) {
        if (!tab) return;
        if (focused_tab == tab) return;
        if (focused_tab) focused_tab->data->unfocus();
        focused_tab = tab;
        focused_tab->data->focus();
    }

    // void close_all() {
    //     focused_tab = nullptr;
    //     for (ListNode<T>* itr = tabs.head; itr; itr = itr->next) {
    //         delete itr->data;
    //         itr->data = nullptr;
    //     }
    //     tabs.remove_all();
    //     init();
    // }

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

    // void close_tab() {
    //     if (!focused_tab) {
    //         return;
    //     }

    //     ListNode<T>* new_tab = focused_tab->next;
    //     if (!new_tab) {
    //         new_tab = focused_tab->prev;
    //     }

    //     delete focused_tab->data;
    //     tabs.remove(focused_tab);

    //     focused_tab = new_tab;

    //     if (!new_tab) {
    //         init();
    //     }
    // }

    T get_focused_window()
    {
        return focused_tab ? focused_tab->data : nullptr;
    }

    void focus() {
        if (!focused_tab) return;
        get_focused_window()->focus();
    }

    void unfocus() {
        if (!focused_tab) return;
        get_focused_window()->unfocus();
    }

    void draw() {
        if (!focused_tab) return;
        get_focused_window()->draw();
    }

    void show() {
        if (!focused_tab) return;
        get_focused_window()->show();
    }

    void resize(Dimension d) {
        bounds = d;
        for (ListNode<T>* node = tabs.head; node; node = node->next) {
            node->data->resize(d);
        }
    }
};
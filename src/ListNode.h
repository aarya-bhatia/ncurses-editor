#pragma once

#include <assert.h>
template<typename T>
struct ListNode
{
    ListNode<T>* prev = nullptr;
    ListNode<T>* next = nullptr;
    T data;

    ListNode(T &_data, ListNode<T>* _prev = nullptr, ListNode<T>* _next = nullptr) {
        prev = _prev;
        next = _next;
        data = _data;

        assert_ok();
    }

    void assert_ok() {
        assert(prev->next == this);
        assert(next->prev == this);
        assert(this->next == next);
        assert(this->prev == prev);
    }

    ~ListNode() {
    }

    void list_size() const { return count_backward() + count_forward() - 1; }
    T& get_data() { return data; }
    void set_data(T& _data) { data = _data; }

    void detach() {
        if (prev) {
            prev->next = next;
        }
        if (next) {
            next->prev = prev;
        }
        prev = next = nullptr;
    }

    void insert_after(ListNode<T>* other) {
        other->next = next;
        if (next) {
            next->prev = other;
        }
        next = other;
        other->prev = this;
    }

    void insert_before(ListNode<T>* other) {
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

        ListNode<T>* tmp = next;
        next = tmp->next;
        if (next) next->prev = this;
        delete tmp;
    }

    void remove_before() {
        if (!prev) {
            return;
        }

        ListNode<T>* tmp = prev;
        prev = tmp->prev;
        if (prev) prev->next = this;
        delete tmp;
    }

    void remove_all_after()
    {
        ListNode<T>* itr = next;
        while (itr) {
            ListNode<T>* tmp = itr->next;
            delete itr;
            itr = tmp;
        }
        next = nullptr;
    }

    void remove_all_before()
    {
        ListNode<T>* itr = prev;
        while (itr) {
            ListNode<T>* tmp = itr->prev;
            delete itr;
            itr = tmp;
        }
        prev = nullptr;
    }

};

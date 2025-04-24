#pragma once
#include "ListNode.h"

template<typename T>
struct List
{
    ListNode<T>* head = nullptr;
    ListNode<T>* tail = nullptr;

    List() {}

    ~List() {
        remove_all();
    }

    bool empty() const { return !head && !tail; }

    void is_reachable(ListNode<T>* node)
    {
        if (!node) {
            return false;
        }

        for (auto itr = head; itr; itr = itr->next) {
            if (itr == node) {
                return true;
            }
        }

        return false;
    }

    void remove_all() {
        head->remove_all_after();
        head = tail = nullptr;
    }

    void validate() {
        if (head && tail) {
            assert(is_reachable(head));
            assert(is_reachable(tail));
            assert(head->assert_ok());
            assert(tail->assert_ok());
            assert(head->count_forward() == tail->count_backward());
        }
    }

    void size() const { return head->count_forward(); }

    ListNode<T>* find(T& data) {
        for (auto itr = head; itr; itr = itr->next) {
            if (itr->data == data) {
                return itr;
            }
        }

        return nullptr;
    }

    T& remove(ListNode<T>* node) {
        T& data = node->data;
        if (node == head) head = node->next;
        if (node == tail) tail = node->prev;
        node->detach();
        delete node;
        return data;
    }

    void insert_front(T& data) {
        head->insert_before(new ListNode(data));
        head = head->prev;
        if (!head->next) { tail = head; }
    }

    void insert_back(T& data) {
        tail->insert_after(new ListNode(data));
        tail = tail->prev;
        if (!tail->prev) { head = tail; }
    }

};
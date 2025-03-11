#pragma once

#include "ncurses.h"
#include <list>
#include <string>

class Line
{
  public:
    using list_type = std::list<std::string>;
    using node_iterator = std::list<std::string>::iterator;
    using byte_iterator = std::string::iterator;

    class iterator
    {
      private:
        const list_type *_list;
        node_iterator _node_itr;
        byte_iterator _byte_itr;

      public:
        iterator(const list_type *list, node_iterator node_itr, byte_iterator byte_itr)
            : _list(list), _node_itr(node_itr), _byte_itr(byte_itr)
        {
        }

        iterator(const iterator &other)
        {
            _list = other._list;
            _node_itr = other._node_itr;
            _byte_itr = other._byte_itr;
        }

        iterator &operator=(const iterator &other)
        {
            _list = other._list;
            _node_itr = other._node_itr;
            _byte_itr = other._byte_itr;
            return *this;
        }

        char operator*() { return *_byte_itr; }
        bool operator==(const iterator &other);
        bool operator!=(const iterator &other);

        iterator &operator++();
        iterator &operator--();
        iterator operator++(int);
        iterator operator--(int);
    };

  private:
    std::list<std::string> _nodes;
    node_iterator _current;

  public:
    Line() { _current = _nodes.begin(); }

    iterator begin() { return iterator(&_nodes, _nodes.begin(), _nodes.front().begin()); }
    iterator end() { return iterator(&_nodes, _nodes.end(), _nodes.back().end()); }

    iterator find_forward(iterator current, int type_mask);
    iterator find_backward(iterator current, int type_mask);
    iterator find_begin_word(iterator current);
    iterator find_end_word(iterator current);

    void add_node(const std::string &str) { _nodes.push_back(str); }
    const std::list<std::string> &get_nodes() const { return _nodes; }

    /* LineIterator next_word() const { return *this; } // TODO */
    /* LineIterator prev_word() const { return *this; } // TODO */

    bool empty() const;
    const node_iterator get_cursor() const { return _current; }
    size_t size() const;
    std::string str() const;
    void backspace();
    void clear();
    void remove(size_t start_index, size_t end_index);
    void insert(char byte);
    void set_cursor(size_t index);

    void display(WINDOW *window) const;
};

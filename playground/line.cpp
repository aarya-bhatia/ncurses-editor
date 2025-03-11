#include "line.h"
#include "common.h"
#include <sstream>
#include <string>

Line::iterator &Line::iterator::operator++()
{
    _byte_itr++;
    if (_byte_itr == (*_node_itr).end()) {
        _node_itr++;
        if (_node_itr != _list->end()) {
            _byte_itr = (*_node_itr).begin();
        }
    }

    return *this;
}

Line::iterator &Line::iterator::operator--()
{
    if (_node_itr == _list->end() || _byte_itr == (*_node_itr).begin()) {
        --_node_itr;
        _byte_itr = (*_node_itr).end();
    }

    --_byte_itr;
    return *this;
}

Line::iterator Line::iterator::operator++(int)
{
    iterator old = *this;
    ++*this;
    return old;
}

Line::iterator Line::iterator::operator--(int)
{
    iterator old = *this;
    --*this;
    return old;
}

bool Line::iterator::operator==(const iterator &other)
{
    return _node_itr == other._node_itr && _byte_itr == other._byte_itr;
}

bool Line::iterator::operator!=(const iterator &other)
{
    return !(*this == other);
}

Line::iterator Line::find_forward(iterator current, int type_mask)
{
    for (iterator itr = current; itr != end(); itr++) {
        if (char_type(*itr) & type_mask) {
            return itr;
        }
    }

    return current;
}

Line::iterator Line::find_backward(iterator current, int type_mask)
{
    for (iterator itr = current; itr != end(); itr--) {
        if (char_type(*itr) & type_mask) {
            return itr;
        }
    }

    return current;
}

Line::iterator Line::find_end_word(iterator current)
{
    iterator itr = current;

    for (; itr != end(); itr++) {
        if (char_type(*itr) & ~char_type(*current)) {
            return --itr;
        }
    }

    return --itr;
}

Line::iterator Line::find_begin_word(iterator current)
{
    iterator itr = current;

    while (1) {
        if (char_type(*itr) & ~char_type(*current)) {
            return ++itr;
        }

        if (itr == begin()) {
            return itr;
        }

        itr--;
    }
}

void Line::set_cursor(size_t index)
{
    size_t accum = 0;
    for (node_iterator node_itr = _nodes.begin(); node_itr != _nodes.end(); node_itr++) {
        if (index == accum + node_itr->size()) {
            _current = node_itr;
            return;
        } else if (index >= accum && index < accum + node_itr->size()) {
            node_iterator copy = _nodes.insert(node_itr, *node_itr);
            *copy = copy->substr(0, index - accum);
            *node_itr = node_itr->substr(index - accum);
            _current = copy;
            return;
        } else {
            accum += node_itr->size();
        }
    }

    _nodes.push_back("");
    _current = --_nodes.end();
}

void Line::insert(char byte)
{
    (*_current) += byte;
}

std::string Line::str() const
{
    std::ostringstream buffer;
    for (const std::string &s : _nodes) {
        buffer << s;
    }
    return buffer.str();
}

void Line::display(WINDOW *window) const
{
    for (const std::string &s : _nodes) {
        wprintw(window, "%s", s.c_str());
    }
}

void Line::clear()
{
    _nodes.clear();
    _current = _nodes.begin();
}

size_t Line::size() const
{
    size_t ret = 0;
    for (const std::string &str : _nodes) {
        ret += str.size();
    }
    return ret;
}

void Line::backspace()
{
    if (_nodes.empty()) {
        return;
    }

    if (_current->empty()) {
        _current = _nodes.erase(_current);
        if (_current != _nodes.begin()) {
            --_current;
        }
    }

    (*_current) = _current->substr(0, _current->size() - 1);
}

void Line::remove(size_t start_index, size_t end_index)
{
    size_t index = 0;

    for (node_iterator itr = _nodes.begin(); itr != _nodes.end() && index <= end_index;) {
        size_t next_index = index + itr->size();
        if (index >= start_index) {
            if (index + itr->size() <= end_index) {
                itr = _nodes.erase(itr);
            } else {
                *itr = itr->substr(0, end_index - index);
            }
        } else {
            ++itr;
        }
        index = next_index;
    }

    set_cursor(0);
}

#pragma once

#include "line.h"
#include <list>

class Page
{
  public:
  private:
    std::list<Line> _lines;
    std::list<Line>::iterator _line_itr;

  public:
    Page() { _line_itr = _lines.begin(); }

    Line &get_line()
    {
        if (_lines.empty()) {
            _lines.push_front(Line());
            return _lines.front();
        }

        return *_line_itr;
    }

    void insert_line_below() { _line_itr = _lines.insert(_line_itr, Line()); }
    void insert_line_above() { _line_itr = _lines.insert(++_line_itr, Line()); }
    void delete_line() { _line_itr = _lines.erase(_line_itr); }

    void operator--() { _line_itr--; }
    void operator++() { _line_itr++; }

    size_t size() const { return _lines.size(); }

    void display(WINDOW *window) const
    {
        for (const Line &line : _lines) {
            line.display(window);
            wprintw(window, "\n");
        }
    }
};

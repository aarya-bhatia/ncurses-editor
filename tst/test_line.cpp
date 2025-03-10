#include "common.h"
#include "line.h"
#include "log.h"
#include <assert.h>
#include <cstdio>

int main()
{
    log_info("testing...");

    Line l;
    l.add_node("hello");
    l.add_node(" world");

    assert(l.get_nodes().size() == 2);

    for (Line::iterator itr = l.begin(); itr != l.end(); itr++) {
        printf("%c", *itr);
    }
    printf("\n");

    for (Line::iterator itr = --l.end(); itr != l.begin(); itr--) {
        printf("%c", *itr);
    }
    printf("\n");

    Line::iterator it = l.find_forward(l.begin(), C_WHITESPACE);
    it++;
    assert(*it == 'w');

    it = l.find_backward(--l.end(), C_WHITESPACE);
    it++;
    assert(*it == 'w');

    it = l.find_end_word(l.begin());
    assert(*it == 'o');

    it = l.find_begin_word(it);
    assert(*it == 'h');

    l.set_cursor(0);
    assert(l.get_cursor()->empty());
    l.insert('!');

    printf("%s\n", l.str().c_str());

    l.set_cursor(5);
    l.insert('@');
    l.insert('@');
    printf("%s\n", l.str().c_str());

    l.set_cursor(10);
    l.insert('@');
    printf("%s\n", l.str().c_str());

    return 0;
}

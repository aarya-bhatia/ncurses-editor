#pragma once

#include "WindowNode.h"
#include "WindowTab.h"
#include "Window.h"

class WindowManager {
private:
    std::vector<WindowTab*> tabs;
    int current_tab = -1;
    Dimension bounds;

public:
    WindowManager(Dimension d) : bounds(d) { init(); }
    ~WindowManager() { _destroy(); }

    int count_tabs() const { return tabs.size(); }
    int get_tab_index() const { return current_tab; }

    void init() {
        if (current_tab != -1) { return; }
        log_debug("init window manager");
        WindowTab* tab = new WindowTab(bounds);
        tabs.push_back(tab);
        current_tab = 0;
    }

    void _destroy() {
        log_debug("destroying window manager");
        for (auto& tab : tabs) {
            delete tab;
        }
    }

    void tab_next() {
        if (current_tab < tabs.size() - 1) {
            current_tab++;
        }
        else {
            current_tab = 0;
        }

        redraw();
    }

    void tab_prev() {
        if (current_tab > 0) {
            current_tab--;
        }
        else {
            current_tab = tabs.size() - 1;
        }

        redraw();
    }

    WindowTab* get_current_tab() {
        if (current_tab == -1) {
            return nullptr;
        }
        return tabs[current_tab];
    }

    WindowTab* tab_new() {
        WindowTab* tab = new WindowTab(bounds);
        tabs.push_back(tab);
        current_tab = tabs.size() - 1;
        return tab;
    }

    void draw() {
        if (current_tab != -1) {
            tabs[current_tab]->draw();
        }
    }

    void show() {
        if (current_tab != -1) {
            tabs[current_tab]->show();
        }
    }

    void redraw() {
        if (current_tab != -1) {
            tabs[current_tab]->redraw();
        }
    }

    void resize(Dimension d) {
        for (auto& tab : tabs) {
            tab->resize(d);
        }
    }

};
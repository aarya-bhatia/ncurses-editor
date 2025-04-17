#pragma once

#include "WMNode.h"
#include "Window.h"

struct WindowManager
{
    WMNode *root_node = nullptr;
    WMNode *current_node = nullptr;
    Dimension bounds;

    WindowManager(Dimension d): bounds(d){ init(); }
    ~WindowManager() { if(root_node) delete root_node;}

    void init()
    {
        if(!root_node) {
            root_node = new WMNode(bounds, nullptr);
            current_node = root_node;
        }
    }

    void open(File *file){
        current_node->open_tab(file);
    }

    void open(Window *file_window){
        current_node->open_tab(file_window);
    }

    void splith() { 
        if(!current_node || !current_node->split_allowed()) return; 
        current_node->splith(); current_node = current_node->children[0];
    }

    void splitv() { 
        if(!current_node || !current_node->split_allowed()) return; 
        current_node->splitv(); current_node = current_node->children[0];
    }

    void draw() { if(root_node) root_node->draw(); }

    void show() { if(root_node) root_node->show(); }

    void resize(Dimension d) { 
        bounds = d; 
        if(!root_node) init(); else root_node->resize(d); 
    }

    void focus(WMNode *node) { 
        current_node = node;
    }

    void focus_right() {
        if(current_node) current_node = current_node->find_right_adjacent_node();
    }

    void focus_left() {
        if(current_node) current_node = current_node->find_left_adjacent_node();
    }

    void focus_top() {
        if(current_node) current_node = current_node->find_top_adjacent_node();
    }

    void focus_bottom() {
        if(current_node) current_node = current_node->find_bottom_adjacent_node();
    }
}; 
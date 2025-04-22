#pragma once

#include "Types.h"
#include "WMNode.h"
#include "Window.h"
#include <assert.h>

struct WindowManager
{
    WMNode *root_node = nullptr;
    WMNode *current_node = nullptr;
    Dimension bounds;

    std::unordered_map<File*, std::vector<Window *>> file_views;

    WindowManager(Dimension d): bounds(d){ init(); }
    ~WindowManager() { if(root_node) delete root_node;}

    void init()
    {
        if(!root_node) {
            root_node = new WMNode(bounds, nullptr);
            focus_on(root_node);
        }
    }

    std::vector<Window *> &get_file_views(File *file) {
        return file_views[file];
    }

    void open(File *file){
        if(!current_node) {init();}
        Window *view = current_node->open_tab(file);
        file_views[file].push_back(view);
        assert(current_node->get_window() == view);
        assert(current_node->layout == WMNode::Layout::NORMAL);
    }

    void splith() { 
        if(!current_node) init();
        if(!current_node || !current_node->split_allowed()) 
        {
            log_debug("split failed");
            return; 
        }
        current_node->splith(); 
        assert(current_node->layout == WMNode::Layout::HSPLIT);
        focus_on(current_node->get_top_child());
    }

    void splitv() { 
        if(!current_node) init();
        if(!current_node || !current_node->split_allowed()){
            log_debug("split failed");
            return; 
        }
        current_node->splitv(); 
        assert(current_node->layout == WMNode::Layout::VSPLIT);
        focus_on(current_node->get_left_child());
    }

    void draw() { if(root_node) root_node->draw();  }
    void show() { if(root_node) root_node->show(); }

    void resize(Dimension d) { 
        bounds = d; 
        if(!root_node) init(); 
        else root_node->resize(d); 
    }

    void focus_on(WMNode *node) { 
        assert(node);
        if(current_node) {
            current_node->unfocus();
        }
        log_info("new focused node: %s", node->bounds.debug_string().c_str());
        current_node = node;
        current_node->focus();
    }

    void focus_right() {
        if(!current_node) init();
        WMNode *new_node = current_node->find_right_adjacent_node();
        if(new_node) {
            if(new_node->layout != WMNode::Layout::NORMAL){log_warn("illegal focus node");return;}
            focus_on(new_node);
        }
        else{log_warn("did not find any focusable nodes");}
    }

    void focus_left() {
        if(!current_node) init();
        WMNode *new_node = current_node->find_left_adjacent_node();
        if(new_node) {
            if(new_node->layout != WMNode::Layout::NORMAL){log_warn("illegal focus node");return;}
            focus_on(new_node);
        }
        else{log_warn("did not find any focusable nodes");}
    }

    void focus_top() {
        if(!current_node) init();
        WMNode *new_node = current_node->find_top_adjacent_node();
        if(new_node) {
            if(new_node->layout != WMNode::Layout::NORMAL){log_warn("illegal focus node");return;}
            focus_on(new_node);
        }
        else{log_warn("did not find any focusable nodes");}
    }

    void focus_bottom() {
        if(!current_node) init();
        WMNode *new_node = current_node->find_bottom_adjacent_node();
        if(new_node) {
            if(new_node->layout != WMNode::Layout::NORMAL){log_warn("illegal focus node");return;}
            focus_on(new_node);
        }
        else{log_warn("did not find any focusable nodes");}
    }
}; 
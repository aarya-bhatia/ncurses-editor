#pragma once

#include "Window.h"
#include "FileSubscriber.h"
#include "log.h"
#include "Editor.h"

struct FileUpdateHandler : public FileSubscriber {

    Editor& editor;

    FileUpdateHandler(Editor& e) : editor(e) {
    }

    void on_file_reload(File& file) override {
        auto result = editor.file_nodes_map.find(&file);
        if (result == editor.file_nodes_map.end()) {
            return;
        }
        for (WindowNode* node : result->second) {
            if (node && node->content && node->content->get_file() == &file) {
                node->redraw();
            }
        }
    }

    void on_insert_character(File& file, Cursor position, char c) override
    {
        auto result = editor.file_nodes_map.find(&file);
        if (result == editor.file_nodes_map.end()) {
            return;
        }
        for (WindowNode* node : result->second) {
            if (node && node->content && node->content->get_file() == &file) {
                node->content->partial_draw_line(position);
            }
        }
    }

    void on_erase_character(File& file, Cursor position) override {
        auto result = editor.file_nodes_map.find(&file);
        if (result == editor.file_nodes_map.end()) {
            return;
        }
        for (WindowNode* node : result->second) {
            if (node && node->content && node->content->get_file() == &file) {
                node->content->partial_draw_line(position);
            }
        }
    }

    void on_replace_character(File& file, Cursor position) override {
        auto result = editor.file_nodes_map.find(&file);
        if (result == editor.file_nodes_map.end()) {
            return;
        }
        for (WindowNode* node : result->second) {
            if (node && node->content && node->content->get_file() == &file) {
                node->content->partial_draw_character(position);
            }
        }
    }

};
#pragma once
#include "EditorMode.h"

struct InsertMode : public EditorMode {
    InsertMode(Editor* editor) {
        this->editor = editor;
    }
    void handle_event(unsigned c);
    std::string name() { return "INSERT"; }
};
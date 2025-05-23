#pragma once
#include "EditorMode.h"

struct NormalMode : public EditorMode
{
    NormalMode(Editor *editor) {
        this->editor = editor;
    }

    void handle_event(unsigned c);
    void handle_two_key_seq();
    void handle_event_no_file(unsigned c);

    std::string name() { return "NORMAL"; }
}; 
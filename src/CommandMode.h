#pragma once
#include "EditorMode.h"

struct CommandMode : public EditorMode
{
    CommandMode(Editor* editor) {
        this->editor = editor;
    }

    std::string mode_line = "";

    void command(const std::string& command);
    void handle_event(unsigned c);

    std::string name() { return "COMMAND"; }

    std::string get_console_string() {
        return ":" + mode_line;
    }
};
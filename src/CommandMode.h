#pragma once
#include "EditorMode.h"

struct CommandMode : public EditorMode
{
    std::string mode_line = "";

    void handle_event(unsigned c);

    std::string name() { return "COMMAND"; }

    std::string get_console_string() {
        return ":" + mode_line;
    }
};
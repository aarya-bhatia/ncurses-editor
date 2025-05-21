#pragma once

#include <string>

class Editor;

struct EditorMode {
    Editor* editor;
    virtual ~EditorMode() = default;
    virtual void handle_event(unsigned c) = 0;
    virtual std::string name() { return "UNKNOWN"; };
    virtual std::string get_status_string() { return ""; };
    virtual std::string get_console_string() { return ""; };
};
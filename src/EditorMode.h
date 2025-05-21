#pragma once

#include <string>
#include "Dimension.h"

class Editor;

struct EditorMode {
    Editor* editor;
    virtual ~EditorMode() = default;
    virtual void handle_event(unsigned c) = 0;
    virtual std::string name() { return "UNKNOWN"; };
    virtual std::string get_status_string() { return ""; };
    virtual std::string get_console_string() { return ""; };

    virtual void draw();
    virtual void show();
    virtual void resize(Dimension d);
};
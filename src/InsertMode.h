#pragma once
#include "EditorMode.h"

struct InsertMode : public EditorMode {
    void handle_event(unsigned c);
    std::string name() { return "INSERT"; }
};
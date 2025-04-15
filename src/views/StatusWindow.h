#pragma once

#include "NcursesWindow.h"
#include <string>
#include <vector>

struct Editor;

struct StatusWindow
{
    Editor& editor;
    Dimension bounds;
    NcursesWindow window;

    StatusWindow(Editor& editor, Dimension bounds) : editor(editor), bounds(bounds), window(bounds) {}

    std::string get_status();

    void draw();
    void show();
    void resize(Dimension bounds);
};
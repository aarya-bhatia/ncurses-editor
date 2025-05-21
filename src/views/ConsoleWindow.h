#pragma once

#include "NcursesWindow.h"
#include <string>
#include "log.h"

class Editor;

struct ConsoleWindow
{
    Editor& editor;
    Dimension bounds;
    NcursesWindow window;

    ConsoleWindow(Editor& editor, Dimension bounds) : editor(editor), bounds(bounds), window(bounds) {
    }

    void draw();
    void show();
    void resize(Dimension bounds);
};
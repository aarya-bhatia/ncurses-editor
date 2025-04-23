#pragma once

#include "NcursesWindow.h"
#include <string>
#include "log.h"

struct Editor;

struct ConsoleWindow
{
    Editor& editor;
    Dimension bounds;
    NcursesWindow window;

    ConsoleWindow(Editor& editor, Dimension bounds) : editor(editor), bounds(bounds), window(bounds) {
        log_debug("initialising status window");
    }

    void draw();
    void show();
    void resize(Dimension bounds);
};
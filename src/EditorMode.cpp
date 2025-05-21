#include "EditorMode.h"
#include "Editor.h"
#include "log.h"
#include "FileView.h"
#include "Window.h"
#include "WindowManager.h"

void EditorMode::draw()
{
    editor->window_manager.draw();

    // this should not happen ideally
    if (editor->window_manager.get_current_tab()->get_focused_node()->get_content() == nullptr) {
        move(0, 0);
    }
}

void EditorMode::show()
{
    editor->window_manager.show();
}

void EditorMode::resize(Dimension d)
{
    editor->window_manager.redraw();
}
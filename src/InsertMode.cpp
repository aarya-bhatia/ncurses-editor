#include "InsertMode.h"
#include "Editor.h"

void InsertMode::handle_event(unsigned c)
{
    auto file = editor->get_focused_file();

    switch (c)
    {
    case CTRL_ESCAPE:
        if (file) {
            file->cursor_left();
        }
        editor->restore_mode();
        break;

    default:
        if (PRINTABLE(c))
        {
            file->insert_character(c);
        }
    }
}
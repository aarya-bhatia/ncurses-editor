#include "CommandMode.h"
#include "Editor.h"

void CommandMode::handle_event(unsigned c)
{
    switch (c)
    {
    case CTRL_ENTER:
        editor->command(mode_line);
        break;

    case CTRL_BACKSPACE:
    case CTRL_DEL:
        if (mode_line.empty())
        {
            editor->restore_mode();
            break;
        }
        else
        {
            mode_line = mode_line.substr(0, mode_line.size() - 1);
        }
        break;

    default:
        if (PRINTABLE(c))
        {
            mode_line += c;
        }
        break;
    }

}
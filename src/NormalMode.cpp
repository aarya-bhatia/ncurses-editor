#include "NormalMode.h"
#include "Editor.h"

void NormalMode::handle_two_key_seq()
{
    auto file = editor->get_focused_file();
    assert(file);
    assert(file->normal_mode_buffer.size() == 2);

    if (file->normal_mode_buffer == "gg") {
        file->goto_line(0);
    }

    file->normal_mode_buffer = "";
}

void NormalMode::handle_event_no_file(unsigned c)
{
    switch (c) {
    case ':':
        editor->change_mode(COMMAND_MODE);
        return;
    }
}

void NormalMode::handle_event(unsigned c)
{
    auto file = editor->get_focused_file();

    if (!file) {
        handle_event_no_file(c);
        return;
    }

    assert(file);

    if (file->normal_mode_buffer.size() > 0) {
        file->normal_mode_buffer += c;
        handle_two_key_seq();
        return;
    }

    switch (c)
    {
    case 'h':
        file->cursor_left();
        return;

    case 'l':
        file->cursor_right();
        return;

    case 'j':
        file->cursor_down();
        return;

    case 'k':
        file->cursor_up();
        return;

    case '0':
        file->move_begin();
        return;

    case '$':
        file->move_cursor_eol();
        return;

    case 'G':
        file->move_cursor_eof();
        return;

    case 'g':
        file->normal_mode_buffer += c;
        return;

    case 'i':
        editor->change_mode(INSERT_MODE);
        return;

    case CTRL_ESCAPE:
        file->normal_mode_buffer.clear();
        return;

    case ':':
        editor->change_mode(COMMAND_MODE);
        return;
    }
}
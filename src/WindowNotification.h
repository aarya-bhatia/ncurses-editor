#pragma once

#include "WindowTab.h"

using Notification = WindowTab::Visitor;

struct FileReloadNotification : public Notification {
    void operator()(Window* content_window) {
    }
};

struct InsertCharacterNotification : public Notification {
    Cursor insert_at;
    InsertCharacterNotification(Cursor p) : insert_at(p) {}
    void operator()(Window* content_window) {
        content_window->partial_draw_line(insert_at);
    }
};

struct EraseCharacterNotification : public Notification {
    Cursor erase_at;
    EraseCharacterNotification(Cursor p) : erase_at(p) {}
    void operator()(Window* content_window) {
        content_window->partial_draw_line(erase_at);
    }
};

struct ReplaceCharacterNotification : public Notification {
    Cursor replace_at;
    ReplaceCharacterNotification(Cursor p) : replace_at(p) {}
    void operator()(Window* content_window) {
        content_window->partial_draw_character(replace_at);
    }
};

#pragma once

#include "Window.h"
#include "FileSubscriber.h"
#include "log.h"
#include "Editor.h"

struct FileUpdateHandler : public FileSubscriber {

    Editor& editor;

    using Notification = WindowTab::Visitor;

    struct FileReloadNotification : public Notification {
        void operator()(Window* content_window) {
            content_window->redraw();
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


    FileUpdateHandler(Editor& e) : editor(e) {
    }

    void notify_all_file_views(File* file, Notification* notification) {
        assert(notification);
        WindowTab* current_tab = editor.window_manager.get_current_tab();
        current_tab->accept(notification);
        delete notification;
    }

    void _notify_all_file_views(WindowNode* node, File* file, Notification* notification) {
    }

    void on_file_reload(File* file) override {
        notify_all_file_views(file, new FileReloadNotification());
    }

    void on_insert_character(File* file, Cursor position) override
    {
        notify_all_file_views(file, new InsertCharacterNotification(position));
    }

    void on_erase_character(File* file, Cursor position) override {
        notify_all_file_views(file, new EraseCharacterNotification(position));
    }

    void on_replace_character(File* file, Cursor position) override {
        notify_all_file_views(file, new ReplaceCharacterNotification(position));
    }

};
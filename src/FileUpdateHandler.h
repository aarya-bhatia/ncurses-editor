#pragma once

#include "FileViewFactory.h"
#include "Window.h"
#include "WindowNotification.h"
#include "FileSubscriber.h"
#include "log.h"
#include "Editor.h"

struct FileUpdateHandler : public FileSubscriber {

    Editor& editor;

    FileUpdateHandler(Editor& e) : editor(e) {
    }

    struct ViewMatcherPredicate : public WindowTab::Predicate {

        File* file = nullptr;

        ViewMatcherPredicate(File* _file) : file(_file) {}

        bool operator()(Window* content_window) {
            FileView* v = FileViewFactory::get_file_view(content_window);
            if (v) {
                return v->get_file() == file;
            }

            return false;
        }
    };

    void notify_all_file_views(File* file, Notification* notification) {
        assert(notification);
        WindowTab* current_tab = editor.window_manager.get_current_tab();
        WindowTab::Predicate* predicate = new ViewMatcherPredicate(file);
        current_tab->accept(notification, predicate);
        delete notification;
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
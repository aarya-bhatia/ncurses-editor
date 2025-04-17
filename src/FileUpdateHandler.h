#pragma once

#include "WindowManager.h"
#include "Window.h"
#include "FileSubscriber.h"

struct FileUpdateHandler : public FileSubscriber {

    WindowManager& wm;

    FileUpdateHandler(WindowManager& _wm) : wm(_wm) {}

    void on_file_reload(File& file) override {
        std::vector<Window*>& views = wm.get_file_views(&file);
        for (Window* view : views) {
            view->force_redraw();
        }
    }

    void on_insert_character(File& file, Cursor position, char c) override
    {
        std::vector<Window*>& views = wm.get_file_views(&file);
        for (Window* view : views) {
            view->partial_draw_line(position);
        }
    }

    void on_erase_character(File& file, Cursor position)override {
        std::vector<Window*>& views = wm.get_file_views(&file);
        for (Window* view : views) {
            view->partial_draw_line(position);
        }
    }

    void on_replace_character(File& file, Cursor position) override {
        std::vector<Window*>& views = wm.get_file_views(&file);
        for (Window* view : views) {
            view->partial_draw_character(position);
        }
    }

};
#pragma once

#include "BorderView.h"
#include "FileView.h"

struct ViewContainer : public Window
{
    BorderView* boundary = nullptr;
    std::vector<FileView*> file_views;
    int current_view = -1;

    bool focused = false;

    ViewContainer(Dimension d) {
        boundary = new BorderView(d);
    }

    bool is_composite() override { return true; }

    bool switch_file(File* file) override {
        for (int i = 0; i < file_views.size(); i++) {
            if (file_views[i]->get_file() == file) {
                set_view(i);
                return true;
            }
        }

        return false;
    }

    void set_view(int index) {
        if (index == current_view || index < 0 || index >= file_views.size()) { return; }
        if (current_view != -1) {
            file_views[current_view]->unfocus();
        }
        current_view = index;
        file_views[current_view]->focus();
    }

    void open_view(FileView* file_view) {
        assert(file_view);
        Dimension d = get_bounds();
        file_view->resize(Dimension(d.x + 1, d.y + 1, d.width - 2, d.height - 2));
        for (int i = 0; i < file_views.size(); i++)
        {
            if (file_views[i] == file_view) {
                set_view(i);
                return;
            }
        }

        file_views.push_back(file_view);
        set_view(file_views.size() - 1);
    }

    File* get_file() override {
        return boundary->file;
    }

    void set_file(File* _file) {
        boundary->file = _file;
    }

    Dimension get_bounds()override {
        return boundary->bounds;
    }

    ~ViewContainer() {
        delete boundary;
        for (FileView* view : file_views) {
            delete view;
        }
    }

    void draw() override {
        boundary->draw();

        if (current_view != -1) {
            file_views[current_view]->draw();
        }

        if (focused) {
            if (current_view == -1) {
                move(boundary->bounds.y + 1, boundary->bounds.x + 1);
            }
            else {
                file_views[current_view]->draw_cursor();
            }
        }
    }

    void show()override {
        boundary->show();

        if (current_view != -1) {
            file_views[current_view]->show();
        }
    }

    void resize(Dimension d)override {
        boundary->resize(d);

        for (FileView* view : file_views) {
            view->resize(Dimension(d.x + 1, d.y + 1, d.width - 2, d.height - 2));
        }
    }

    void redraw() override {
        boundary->redraw();

        if (current_view != -1) {
            file_views[current_view]->redraw();
        }
    }

    void focus() override {
        focused = true;
        if (current_view != -1) {
            file_views[current_view]->focus();
        }
    }

    void unfocus() override {
        focused = false;
        if (current_view != -1) {
            file_views[current_view]->unfocus();
        }
    }

    void partial_draw_line(Cursor position) override
    {
        if (current_view != -1) {
            file_views[current_view]->partial_draw_line(position);
        }
    }

    void partial_draw_character(Cursor position) override
    {
        if (current_view != -1) {
            file_views[current_view]->partial_draw_character(position);
        }
    }
};
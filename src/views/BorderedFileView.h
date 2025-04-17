#pragma once

#include "FileView.h"

struct BorderedFileView : public Window {
    Dimension bounds;
    FileView* file_view = nullptr;
    NcursesWindow frame;

    BorderedFileView(FileView* file_view = nullptr) {
        init(file_view);
    }

    ~BorderedFileView() {
        delete file_view;
    }

    void init(FileView* file_view) {
        if (!file_view) {
            return;
        }
        this->file_view = file_view;
        bounds = file_view->bounds;
        frame = NcursesWindow(bounds);
        if (file_view) {
            file_view->resize(Dimension(bounds.x + 1, bounds.y + 1, bounds.width - 2, bounds.height - 2));
        }
    }

    void resize(Dimension d) override
    {
        bounds = d;
        frame = NcursesWindow(bounds);
        if (file_view) {
            file_view->resize(Dimension(bounds.x + 1, bounds.y + 1, bounds.width - 2, bounds.height - 2));
        }
    }

    void draw() override
    {
        frame.draw_border();
        if (file_view) {
            file_view->draw();
        }
    }

    void show() override
    {
        frame.show();
        if (file_view) {
            file_view->show();
        }
    }

    void focus()  override {
        if (file_view) {
            file_view->focus();
        }
    }

    void unfocus()  override {
        if (file_view) {
            file_view->unfocus();
        }
    }

    Window* copy(Dimension d)  override {
        BorderedFileView* w = new BorderedFileView((FileView*)file_view->copy(d));
        w->resize(d);
        return w;
    }
};
#pragma once

#include "FileView.h"

struct BorderedFileView: public Window {
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
        resize(bounds);
    }

    void resize(Dimension d)
    {
        bounds = d;
        frame = NcursesWindow(bounds);
        if (file_view) {
            file_view->resize(Dimension(bounds.x + 1, bounds.y + 1, bounds.width - 2, bounds.height - 2));
        }
    }

    void draw()
    {
        frame.draw_border();
        if (file_view) {
            file_view->draw();
        }
    }

    void show()
    {
        frame.show();
        if (file_view) {
            file_view->show();
        }
    }

    void focus() {
        if(file_view) {
            file_view->focus();
        }
    }

    void unfocus() {
        if(file_view) {
            file_view->unfocus();
        }
    }

    Window* copy(Dimension d) {
        BorderedFileView *w = new BorderedFileView((FileView *) file_view->copy(d));
        w->resize(d);
        return w;
    }
};
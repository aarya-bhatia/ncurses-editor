#pragma once

#include "FileView.h"

struct BorderedFileView : public Window {
    Dimension bounds;
    FileView* file_view = nullptr;
    NcursesWindow frame;

    BorderedFileView(File *file, Dimension d){
        bounds = d;
        init(file, d);
    }

    ~BorderedFileView() {
        delete file_view;
    }

    void init(File* file, Dimension bounds) {
        this->bounds = bounds;
        Dimension child_bounds(bounds.x + 1, bounds.y + 1, bounds.width - 2, bounds.height - 2);
        delete file_view;
        file_view = new FileView(file, child_bounds);
        frame = NcursesWindow(bounds);
    }

    void resize(Dimension d) override
    {
        init(file_view->file, d);
    }

    void draw() override
    {
        frame.draw_border();
        file_view->draw();
    }

    void show() override
    {
        frame.show();
        file_view->show();
    }

    void focus()  override {
        file_view->focus();
    }

    void unfocus()  override {
        file_view->unfocus();
    }

    Window* copy(Dimension d)  override {
        return new BorderedFileView(file_view->file, bounds);
    }

    File* get_file() override { return file_view->file; }
};
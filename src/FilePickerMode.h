#pragma once
#include "EditorMode.h"
#include "File.h"

struct FilePickerMode : public EditorMode
{
    File* _file;

    FilePickerMode(Editor* editor);
    ~FilePickerMode();

    void handle_event(unsigned c);
};
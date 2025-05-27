#pragma once
#include "EditorMode.h"
#include "NormalMode.h"
#include "File.h"

struct FilePickerMode : public EditorMode
{
    File* _file = nullptr;
    NormalMode _normal;

    FilePickerMode(Editor* editor);
    ~FilePickerMode();

    void handle_event(unsigned c);
};
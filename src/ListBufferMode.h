#pragma once
#include <sstream>
#include "Editor.h"
#include "EditorMode.h"
#include "NormalMode.h"
#include "File.h"

struct ListBufferMode : public EditorMode
{
    File* _file = nullptr;
    NormalMode _normal;

    std::string file_description(File* file) const {
        std::ostringstream ss;

        if (!file->filename.empty()) {
            ss << file->filename;
        }
        else {
            ss << "Untitled";
        }

        ss << " ";
        ss << "Ln:" << 1 + file->cursor.y << "/" << file->lines.size();
        ss << " ";
        ss << "Col:" << 1 + file->cursor.x << "/" << file->cursor.line->size();

        return ss.str();
    }

    ListBufferMode(Editor* editor) : _normal(editor)
    {
        this->editor = editor;
        _file = new File();

        _file->lines.clear();

        for (File* f : editor->files) {
            std::string s = file_description(f);
            _file->lines.emplace_back(s.begin(), s.end());
        }

        if (_file->lines.empty())
        {
            _file->lines.push_back({});
        }

        _file->cursor.x = 0;
        _file->cursor.y = 0;
        _file->cursor.line = _file->lines.begin();
        _file->cursor.col = _file->cursor.line->begin();

        editor->open(_file);
    }

    ~ListBufferMode() {
        delete _file;
    }

    void handle_event(unsigned c)
    {
        switch (c) {
        case CTRL_ENTER:
            return;
        default:
            _normal.handle_event(c);
        }
    }
};
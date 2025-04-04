#pragma once

#include <string>
#include "File.h"

struct Painter
{
    File* file = nullptr;

    Painter(File* file) : file(file) {}
    ~Painter();

    void RedrawFile();
    void RedrawLine(int y, const std::string& content);
    void InsertCharacter(int y, int x, char c);
    void ReplaceCharacter(int y, int x, char c);
    void EraseLine(int y);
    void Clear();
    void ClearLine(int y);
    void ClearPartial(int y, int x);
    void Move(int y, int x);

private:
    bool isLineVisible(int line)
    {
        return file->getDisplayY(line) >= 0 && line < file->getMaxY();
    }

    void showCursor()
    {
        if (file->adjustScroll()) {
            RedrawFile();
        }

        wmove(file->window, file->getDisplayY(file->cursor.y), file->getDisplayX(file->cursor.x));
    }
};
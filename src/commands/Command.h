#pragma once
struct Editor;

struct Command {
    virtual ~Command() = default;
    virtual int execute(Editor& editor) = 0;
};
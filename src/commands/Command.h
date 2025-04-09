#pragma once

#include <string>

struct Editor;

enum class Status
{
    ok = 0,
    internal_error,
    error_file_unavailable,
    error_illegal_arguments,
    error_illegal_operation,
    error_not_implemented
};

struct Result
{
    Status status = Status::ok;
    std::string message = "";

    Result(Status status = Status::ok, std::string message = "") :
        status(status), message(message) {
    }
};

struct Command {

    virtual ~Command() = default;
    virtual Result execute(Editor& editor) = 0;

    // TODO: virtual bool undoable() = 0;
    // TODO: virtual void undo();
};
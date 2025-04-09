#pragma once
#include "Command.h"

struct LoadFileCommand: public Command
{
    Result execute(Editor &editor) override;
};
#include "LoadFileCommand.h"
#include "editor.h"

Result LoadFileCommand::execute(Editor &editor)
{
    if(!editor.get_current_file()){
        return Status::error_file_unavailable;
    }

    return Result(Status::error_not_implemented);
}
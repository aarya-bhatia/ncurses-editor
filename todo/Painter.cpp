#pragma once

#include "Painter.h"
#include <ncurses.h>

struct Painter
{
    void RedrawFile(WindowContext* ctx);

    void RedrawLine(WindowContext* ctx, int y, const std::string& content)
    {
        if (!isLineVisible(ctx, y)) {
            return;
        }

        y = ctx->getDisplayY(y);
        wmove(ctx->window, y, 0);
        wclrtoeol(ctx->window);

        if (content.size() >= ctx->getMaxX()) {
            waddstr(ctx->window, content.substr(0, ctx->getMaxX()).c_str());
        }
        else {
            waddstr(ctx->window, content.c_str());
        }
    }

    void InsertCharacter(WindowContext* ctx, int y, int x, char c)
    {
        if (!isLineVisible(ctx, y)) {
            return;
        }

        y = ctx->getDisplayY(y);
        x = ctx->getDisplayX(x);

        if (x >= 0 && x < ctx->getMaxX()) {
            wmove(ctx->window, y, x);
            waddch(ctx->window, c);
        }

        ctx->moveCursorX(1);
        showCursor(ctx);
    }

    void ReplaceCharacter(WindowContext* ctx, int y, int x, char c) = 0;
    void EraseLine(WindowContext* ctx, int y) = 0;
    void Clear(WindowContext* ctx) = 0;
    void ClearLine(WindowContext* ctx, int y) = 0;
    void ClearPartial(WindowContext* ctx, int y, int x) = 0;
    void Move(WindowContext* ctx, int y, int x) = 0;

};
#pragma once
#include "Window.h"

struct ITestWindow
{
    virtual ~ITestWindow() = default;
    virtual void resize(Dimension d) {}
    virtual void draw() {}
    virtual void show() {}
    virtual void clear() {}
    virtual void focus() {}
    virtual void unfocus() {}
};

struct TestWindow : public ITestWindow
{
    int id;

    void resize(Dimension d) {}
    void draw() {}
    void show() {}
    void clear() {}
    void focus() {}
    void unfocus() {}
};
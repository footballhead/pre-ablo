#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

class IComponent {
    virtual ~IComponent() = default;

    virtual void show() = 0;
    virtual void hide() = 0;
    virtual bool is_shown() const = 0;

    virtual void draw() const = 0;
    virtual void wnproc(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) = 0;
};

#include <windows.h>
#include "my_platform.h"

Vector2I GetMouseMonitorPosition(void)
{
    POINT point;
    if(GetCursorPos(&point))
        return { static_cast<int>(point.x), static_cast<int>(point.y) };
    return { 0, 0 };
}

bool GetMouseKeyLeft() {
    return (GetKeyState(VK_LBUTTON) & 0x8000) != 0;
}

bool GetMouseKeyRight() {
    return (GetKeyState(VK_RBUTTON) & 0x8000) != 0;
}

bool GetMouseKeyMiddle() {
    return (GetKeyState(VK_MBUTTON) & 0x8000) != 0;
}

bool GetMouseWhell() {
    return false;
}
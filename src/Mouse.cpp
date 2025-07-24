#include "Mouse.hpp"

#ifdef GEODE_IS_WINDOWS
#include <windows.h>
#else
#ifdef GEODE_IS_MACOS
#include <CoreGraphics/CGEventSource.h>
#endif
#endif

Mouse* UMouse = new Mouse;

Mouse* Mouse::get() {
    return UMouse;
};

bool Mouse::isMiddleClickPressed()
{
    #ifdef GEODE_IS_WINDOWS
        return (GetAsyncKeyState(VK_MBUTTON) & 0x8000);
    #else
    #ifdef GEODE_IS_MACOS
        return CGEventSourceButtonState(kCGEventSourceStateHIDSystemState, kCGMouseButtonCenter);
    #else
        return false;
    #endif
    #endif
}

#ifdef GEODE_IS_WINDOWS
void Mouse::setCursorForDirection(MouseDrag::Direction dir)
{
	HCURSOR cursor = NULL;

	switch (dir)
	{
	case MouseDrag::UP:
		cursor = LoadCursor(NULL, MAKEINTRESOURCE(32655));
		break;
	case MouseDrag::DOWN:
		cursor = LoadCursor(NULL, MAKEINTRESOURCE(32656));
		break;
	case MouseDrag::RIGHT:
		cursor = LoadCursor(NULL, MAKEINTRESOURCE(32657));
		break;
	case MouseDrag::LEFT:
		cursor = LoadCursor(NULL, MAKEINTRESOURCE(32658));
		break;
	case MouseDrag::NONE:
	default:
		cursor = LoadCursor(NULL, MAKEINTRESOURCE(32654));
		break;
	}

	if (cursor)
	{
		SetCursor(cursor);
	}
}

void Mouse::resetCursor()
{
	SetCursor(LoadCursor(NULL, IDC_ARROW));
}
#else
void Mouse::resetCursor()
{
    return;
}
void Mouse::setCursorForDirection(MouseDrag::Direction dir) {
    return;
}
#endif

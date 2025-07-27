#include "Mouse.hpp"

#ifdef GEODE_IS_WINDOWS
#include <windows.h>
#elif defined(GEODE_IS_MACOS)
#include <CoreGraphics/CGEventSource.h>
#endif

Mouse* UMouse = new Mouse;

Mouse* Mouse::get() {
    return UMouse;
};

MouseDrag::MouseKeys Mouse::updateMouseKeys()
{
	#ifdef GEODE_IS_WINDOWS
		#pragma message("Compiling windows")
		m_mouseKeys.MiddleClick = (GetAsyncKeyState(VK_MBUTTON) & 0x8000);
		m_mouseKeys.LeftClick   = (GetAsyncKeyState(VK_LBUTTON) & 0x8000);
		m_mouseKeys.RightClick  = (GetAsyncKeyState(VK_RBUTTON) & 0x8000);
	#elif defined(GEODE_IS_MACOS)
		#pragma message("Compiling macos")
		m_mouseKeys.MiddleClick = CGEventSourceButtonState(kCGEventSourceStateHIDSystemState, kCGMouseButtonCenter);
		m_mouseKeys.LeftClick   = CGEventSourceButtonState(kCGEventSourceStateHIDSystemState, kCGMouseButtonLeft);
		m_mouseKeys.RightClick  = CGEventSourceButtonState(kCGEventSourceStateHIDSystemState, kCGMouseButtonRight);
	#else
		m_mouseKeys.MiddleClick = false;
		m_mouseKeys.LeftClick   = false;
		m_mouseKeys.RightClick  = false;
	#endif
	return m_mouseKeys;
}

#ifdef GEODE_IS_WINDOWS
std::array<HCURSOR, 6> m_cursorCache;
void Mouse::setupCache() {
	m_cursorCache[MouseDrag::UP]    = LoadCursor(NULL, MAKEINTRESOURCE(32655));
	m_cursorCache[MouseDrag::DOWN]  = LoadCursor(NULL, MAKEINTRESOURCE(32656));
	m_cursorCache[MouseDrag::RIGHT] = LoadCursor(NULL, MAKEINTRESOURCE(32657));
	m_cursorCache[MouseDrag::LEFT]  = LoadCursor(NULL, MAKEINTRESOURCE(32658));
	m_cursorCache[MouseDrag::ALL]   = LoadCursor(NULL, MAKEINTRESOURCE(32654));
	m_cursorCache[MouseDrag::DEFAULT]  = LoadCursor(NULL, IDC_ARROW);
}
void Mouse::setCursorForDirection(MouseDrag::Direction dir)
{
	m_direction = dir;
    auto cursor = m_cursorCache[dir];
    SetCursor(cursor ? cursor : m_cursorCache[MouseDrag::DEFAULT]);
}

#elif defined(IAMAPPLE)
std::array<void*, 6> m_cursorCache;
void Mouse::setupCache() {
	m_cursorCache[MouseDrag::UP]    =  mmloadCursor(geode::prelude::Mod::get()->getResourcesDir() / "up.png");
	m_cursorCache[MouseDrag::DOWN]  =  mmloadCursor(geode::prelude::Mod::get()->getResourcesDir() / "down.png");
	m_cursorCache[MouseDrag::RIGHT] =  mmloadCursor(geode::prelude::Mod::get()->getResourcesDir() / "right.png");
	m_cursorCache[MouseDrag::LEFT]  =  mmloadCursor(geode::prelude::Mod::get()->getResourcesDir() / "left.png");
	m_cursorCache[MouseDrag::ALL]   = mmloadCursor(geode::prelude::Mod::get()->getResourcesDir() / "all.png");
	m_cursorCache[MouseDrag::DEFAULT]  = mmDefaultCursor();
}

void Mouse::setCursorForDirection(MouseDrag::Direction dir)
{
	m_direction = dir;
    auto cursor = m_cursorCache[dir];
    mmsetCursor(cursor ? cursor : m_cursorCache[MouseDrag::DEFAULT]);
};

#else
void Mouse::resetCursor()
{
    return;
}
void Mouse::setupCache() {

}
void Mouse::setCursorForDirection(MouseDrag::Direction dir)
{
	m_direction = dir;
}
#endif

void Mouse::resetCursor()
{
	setCursorForDirection(MouseDrag::DEFAULT);
};

MouseDrag::Direction Mouse::getDirection() {
	return m_direction;
};
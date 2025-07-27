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
		m_mouseKeys.MiddleClick = (GetAsyncKeyState(VK_MBUTTON) & 0x8000);
		m_mouseKeys.LeftClick   = (GetAsyncKeyState(VK_LBUTTON) & 0x8000);
		m_mouseKeys.RightClick  = (GetAsyncKeyState(VK_RBUTTON) & 0x8000);
	#elif defined(GEODE_IS_MACOS)
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

#elif defined(__APPLE__)
std::array<void*, 6> m_cursorCache;
void Mouse::setupCache() {
	auto ModResources = geode::prelude::Mod::get()->getResourcesDir();
	m_cursorCache[MouseDrag::UP]    =  mmloadCursor(ModResources/ "up.png",ModResources/ "up@32.png",ModResources/ "up@128.png");
	m_cursorCache[MouseDrag::DOWN]  =  mmloadCursor(ModResources/ "down.png",ModResources/ "down@32.png",ModResources/ "down@128.png");
	m_cursorCache[MouseDrag::RIGHT] =  mmloadCursor(ModResources/ "right.png",ModResources/ "right@32.png",ModResources/ "right@128.png");
	m_cursorCache[MouseDrag::LEFT]  =  mmloadCursor(ModResources/ "left.png",ModResources/ "left@32.png",ModResources/ "left@128.png");
	m_cursorCache[MouseDrag::ALL]   = mmloadCursor(ModResources/ "all.png",ModResources/ "all@32.png",ModResources/ "all@128.png");
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
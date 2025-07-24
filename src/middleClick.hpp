
#ifdef GEODE_IS_WINDOWS
#include <windows.h>
#else
#ifdef GEODE_IS_MACOS
#include <ApplicationServices/ApplicationServices.h>
#endif
#endif

static bool isMiddleClickPressed()
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
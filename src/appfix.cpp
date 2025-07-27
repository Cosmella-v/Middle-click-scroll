#include <Geode/modify/AppDelegate.hpp>
#include "Mouse.hpp"

using namespace geode::prelude;

Mouse* mouse = Mouse::get();

class $modify(MyAppDelegate, AppDelegate) {
	
	void applicationWillResignActive() {
        mouse->m_windowActive = false;
		AppDelegate::applicationWillResignActive();
	}
	void applicationDidEnterBackground() {
        mouse->m_windowActive = false;
		AppDelegate::applicationDidEnterBackground();
	}
    void applicationWillBecomeActive() {
        mouse->m_windowActive = true;
        AppDelegate::applicationWillBecomeActive();
    }
    void applicationWillEnterForeground() {
        mouse->m_windowActive = true;
        AppDelegate::applicationWillEnterForeground();
    }
};


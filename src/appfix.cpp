#include <Geode/modify/AppDelegate.hpp>
#include "Mouse.hpp"

using namespace geode::prelude;

Mouse* mouse = Mouse::get();

class $modify(MyAppDelegate, AppDelegate) {
	
	void applicationWillResignActive() {
        if (mouse) {
            mouse->m_windowActive = false;
        } else {
            mouse = Mouse::get();
        }
		AppDelegate::applicationWillResignActive();
	}
	void applicationDidEnterBackground() {
        if (mouse) {
            mouse->m_windowActive = false;
        } else {
            mouse = Mouse::get();
        }
		AppDelegate::applicationDidEnterBackground();
	}
    void applicationWillBecomeActive() {
        if (mouse) {
            mouse->m_windowActive = true;
        } else {
            mouse = Mouse::get();
        }
        AppDelegate::applicationWillBecomeActive();
    }
    void applicationWillEnterForeground() {
        if (mouse) {
            mouse->m_windowActive = true;
        } else {
            mouse = Mouse::get();
        }
        AppDelegate::applicationWillEnterForeground();
    }
};


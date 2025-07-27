#pragma once
#include <string>

namespace MouseDrag {
    enum Direction
    {
        DEFAULT,
        ALL,
        UP,
        DOWN,
        LEFT,
        RIGHT
    };
    struct MouseKeys {
        bool LeftClick = false;
        bool RightClick = false;
        bool MiddleClick = false;
    };
}

class Mouse {
public:
    Mouse() {
        setupCache();
    };
    MouseDrag::MouseKeys updateMouseKeys();
    void setCursorForDirection(MouseDrag::Direction dir);
    MouseDrag::Direction getDirection();
    void resetCursor();
    double m_scrollSpeedFactor = 0.1;
    double m_maxSpeed = 50.0;
    bool m_windowActive = true;
    static Mouse* get();
private:
    #ifdef IAMAPPLE
        void* mmloadCursor(const std::string& imagePath);
        void mmsetCursor(void* cursorPtr);
        void* mmDefaultCursor();
    #endif
    void setupCache();
    MouseDrag::MouseKeys m_mouseKeys;
    MouseDrag::Direction m_direction;
};

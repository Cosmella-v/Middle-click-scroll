#pragma once

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
}

class Mouse {
public:
    Mouse() {
        setupCache();
    };
    bool isMiddleClickPressed();
    void setCursorForDirection(MouseDrag::Direction dir);
    MouseDrag::Direction getDirection();
    void resetCursor();
    double m_scrollSpeedFactor = 0.1;
    double m_maxSpeed = 50.0;
    bool m_windowActive = true;
    static Mouse* get();
private:
    #ifdef GEODE_IS_MACOS
        void* mmloadCursor(const std::string& imagePath);
        void mmsetCursor(void* cursorPtr);
        void* mmDefaultCursor();
    #endif
    void setupCache();
    MouseDrag::Direction m_direction;
};

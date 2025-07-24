#pragma once

namespace MouseDrag {
    enum Direction
    {
        NONE,
        UP,
        DOWN,
        LEFT,
        RIGHT
    };
}

class Mouse {
public:
    bool isMiddleClickPressed();
    void setCursorForDirection(MouseDrag::Direction dir);
    void resetCursor();
    static Mouse* get();
};

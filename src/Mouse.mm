#import <Cocoa/Cocoa.h>
#include "Mouse.hpp"

void Mouse::mmsetCursor(void* cursorPtr) {
    if (cursorPtr == nullptr) {
        NSLog(@"[MacCursorManager::setCursor] Null cursor pointer");
        return;
    }
    NSCursor* cursor = reinterpret_cast<NSCursor*>(cursorPtr);
    [cursor set];
}
void* Mouse::mmDefaultCursor(MouseDrag dir) {
    NSCursor* cursor = [NSCursor arrowCursor];
    return reinterpret_cast<void*>(cursor);
}
void* Mouse::mmloadCursor(const std::string& imagePath) {
        NSString* nsPath = [NSString stringWithUTF8String:imagePath.c_str()];
        NSImage* image = [[NSImage alloc] initWithContentsOfFile:nsPath];
        if (!image || ![image isValid]) {
            NSLog(@"Failed to load image at path: %@", nsPath);
            return nullptr;
        }

        NSSize size = image.size;
        NSPoint hotSpot = NSMakePoint(size.width / 2.0, size.height / 2.0);

        NSCursor* cursor = [[NSCursor alloc] initWithImage:image hotSpot:hotSpot];
        return reinterpret_cast<void*>(cursor);
}


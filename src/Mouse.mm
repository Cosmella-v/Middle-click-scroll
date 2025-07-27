#import <Cocoa/Cocoa.h>
#include <string>
#include "Mouse.hpp"

void Mouse::mmsetCursor(void* cursorPtr) {
    if (cursorPtr == nullptr) {
        NSLog(@"[MacCursorManager::setCursor] Null cursor pointer");
        return;
    }
    NSCursor* cursor = reinterpret_cast<NSCursor*>(cursorPtr);
    [cursor set];
}
void* Mouse::mmDefaultCursor() {
    NSCursor* cursor = [NSCursor arrowCursor];
    return reinterpret_cast<void*>(cursor);
}

void* Mouse::mmloadCursor(const std::string& imagePath32, const std::string& imagePath64, const std::string& imagePath128) {
    NSString* nsPath32 = [NSString stringWithUTF8String:imagePath32.c_str()];
    NSImage* image32 = [[NSImage alloc] initWithContentsOfFile:nsPath32];
    if (!image32 || ![image32 isValid]) {
        NSLog(@"Failed to load 32x32 image at path: %@", nsPath32);
        return nullptr;
    }

    NSString* nsPath64 = [NSString stringWithUTF8String:imagePath64.c_str()];
    NSImage* image64 = [[NSImage alloc] initWithContentsOfFile:nsPath64];
    if (!image64 || ![image64 isValid]) {
        NSLog(@"Failed to load 64x64 image at path: %@", nsPath64);
        return nullptr;
    }

    NSString* nsPath128 = [NSString stringWithUTF8String:imagePath128.c_str()];
    NSImage* image128 = [[NSImage alloc] initWithContentsOfFile:nsPath128];
    if (!image128 || ![image128 isValid]) {
        NSLog(@"Failed to load 128x128 image at path: %@", nsPath128);
        return nullptr;
    }

    NSImage* combinedImage = [[NSImage alloc] initWithSize:image128.size];

    for (NSImageRep* rep in [image32 representations]) {
        [combinedImage addRepresentation:rep];
    }

    for (NSImageRep* rep in [image64 representations]) {
        NSSize logicalSize = NSMakeSize(rep.pixelsWide / 2.0, rep.pixelsHigh / 2.0);
        [rep setSize:logicalSize];
        [combinedImage addRepresentation:rep];
    }

    for (NSImageRep* rep in [image128 representations]) {
        NSSize logicalSize = NSMakeSize(rep.pixelsWide / 4.0, rep.pixelsHigh / 4.0);
        [rep setSize:logicalSize];
        [combinedImage addRepresentation:rep];
    }

    NSSize size = combinedImage.size;
    NSPoint hotSpot = NSMakePoint(size.width / 2.0, size.height / 2.0);

    NSCursor* cursor = [[NSCursor alloc] initWithImage:combinedImage hotSpot:hotSpot];
    return reinterpret_cast<void*>(cursor);
}

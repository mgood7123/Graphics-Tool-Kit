//
//  OpenGLRenderer.m
//  OpenGLApp
//
//  Created by Matthew Good on 28/7/21.
//
// https://developer.apple.com/library/archive/samplecode/GLEssentials/Listings/

#import "OpenGLRenderer.h"

//#include <Views/Views.h>

@implementation OpenGLRenderer

- (instancetype) init {
    multiTouch.setMaxSupportedTouches(10);
    appInstance = new AppInstance();
    return self;
}

- (void)touchesBeganWithTouch:(NSTouch *)touch AndPoint:(NSPoint)point {
    multiTouch.addTouch(
        ((NSInteger) touch.identity),
        point.x,
        point.y
    );
    appInstance->onTouchEvent(multiTouch);
}

- (void)touchesMovedWithTouch:(NSTouch *)touch AndPoint:(NSPoint)point {
    multiTouch.moveTouch(
        ((NSInteger) touch.identity),
        point.x,
        point.y
    );
    appInstance->onTouchEvent(multiTouch);
}

- (void)touchesEndedWithTouch:(NSTouch *)touch AndPoint:(NSPoint)point {
    multiTouch.removeTouch(
        ((NSInteger) touch.identity),
        point.x,
        point.y
    );
    appInstance->onTouchEvent(multiTouch);
}

- (void)touchesCancelledWithTouch:(NSTouch *)touch AndPoint:(NSPoint)point {
    multiTouch.cancelTouch(
        ((NSInteger) touch.identity),
        point.x,
        point.y
    );
    appInstance->onTouchEvent(multiTouch);
}

- (void) resizeWithWidth:(GLuint)width AndHeight:(GLuint)height

{
    appInstance->surfaceChanged(width, height);
}

- (void) render

{
    appInstance->onDraw();
}

- (void) destroy
{
    delete appInstance;
}

@end

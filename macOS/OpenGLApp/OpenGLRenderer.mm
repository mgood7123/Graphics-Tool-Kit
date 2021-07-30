//
//  OpenGLRenderer.m
//  OpenGLApp
//
//  Created by Matthew Good on 28/7/21.
//
// https://developer.apple.com/library/archive/samplecode/GLEssentials/Listings/

#import "OpenGLRenderer.h"

@implementation OpenGLRenderer

- (instancetype) init {
    multiTouch.setMaxSupportedTouches(10);
    return self;
}

- (void)touchesBeganWithEvent:(NSSet *) touches {
    NSArray *array = [touches allObjects];
    NSUInteger numberOfTouches = [array count];
    for (unsigned long i = 0; i < numberOfTouches; i++) {
        NSTouch *t = [array objectAtIndex:i];
        multiTouch.addTouch(
            ((NSInteger) t.identity),
            t.normalizedPosition.x,
            t.normalizedPosition.y
        );
        appInstance.onTouchEvent(multiTouch);
    }
}

- (void)touchesMovedWithEvent:(NSSet *) touches {
    NSArray *array = [touches allObjects];
    NSUInteger numberOfTouches = [array count];
    for (unsigned long i = 0; i < numberOfTouches; i++) {
        NSTouch *t = [array objectAtIndex:i];
        multiTouch.moveTouch(
            ((NSInteger) t.identity),
            t.normalizedPosition.x,
            t.normalizedPosition.y
        );
        appInstance.onTouchEvent(multiTouch);
    }
}

- (void)touchesEndedWithEvent:(NSSet *) touches {
    NSArray *array = [touches allObjects];
    NSUInteger numberOfTouches = [array count];
    for (unsigned long i = 0; i < numberOfTouches; i++) {
        NSTouch *t = [array objectAtIndex:i];
        multiTouch.removeTouch(
            ((NSInteger) t.identity),
            t.normalizedPosition.x,
            t.normalizedPosition.y
        );
        appInstance.onTouchEvent(multiTouch);
    }
}

- (void)touchesCancelledWithEvent:(NSSet *) touches {
    NSArray *array = [touches allObjects];
    NSUInteger numberOfTouches = [array count];
    for (unsigned long i = 0; i < numberOfTouches; i++) {
        NSTouch *t = [array objectAtIndex:i];
        multiTouch.cancelTouch(
            ((NSInteger) t.identity),
            t.normalizedPosition.x,
            t.normalizedPosition.y
        );
        appInstance.onTouchEvent(multiTouch);
    }
}

- (void) resizeWithWidth:(GLuint)width AndHeight:(GLuint)height

{
    appInstance.surfaceChanged(width, height);
}

- (void) render

{
    appInstance.onDraw();
}

- (void) destroy
{
    appInstance.destroyResources();
}

@end

//
//  OpenGLRenderer.h
//  MacOS
//
//  Created by Matthew Good on 28/7/21.
//
// https://developer.apple.com/library/archive/samplecode/GLEssentials/Listings/

#ifndef OpenGLRenderer_h
#define OpenGLRenderer_h

#import <Foundation/Foundation.h>

#define PLATFORM_MACOS 1

#if TARGET_IOS

#import <OpenGLES/ES2/gl.h>
#import <OpenGLES/ES2/glext.h>

#else

#import <OpenGL/OpenGL.h>

// OpenGL 3.2 is only supported on MacOS X Lion and later
// CGL_VERSION_1_3 is defined as 1 on MacOS X Lion and later
#if CGL_VERSION_1_3
// Set to 0 to run on the Legacy OpenGL Profile
#define ESSENTIAL_GL_PRACTICES_SUPPORT_GL3 1
#else
#define ESSENTIAL_GL_PRACTICES_SUPPORT_GL3 0
#endif //!CGL_VERSION_1_3

#if ESSENTIAL_GL_PRACTICES_SUPPORT_GL3
#import <OpenGL/gl3.h>
#else
#import <OpenGL/gl.h>
#endif //!ESSENTIAL_GL_PRACTICES_SUPPORT_GL3

#endif // !TARGET_IOS

#import <Cocoa/Cocoa.h> // NSTouch
#include <AppInstance/AppInstance.h>

@interface OpenGLRenderer : NSObject
{
    MultiTouch multiTouch;
    AppInstance * appInstance;
}

- (instancetype) init;

- (void) touchesBeganWithTouch:(NSTouch *)touch AndPoint:(NSPoint)point;
- (void) touchesMovedWithTouch:(NSTouch *)touch AndPoint:(NSPoint)point;
- (void) touchesEndedWithTouch:(NSTouch *)touch AndPoint:(NSPoint)point;
- (void) touchesCancelledWithTouch:(NSTouch *)touch AndPoint:(NSPoint)point;

- (void) resizeWithWidth:(GLuint)width AndHeight:(GLuint)height;
- (void) render;
- (void) destroy;

@end

#endif /* OpenGLRenderer_h */

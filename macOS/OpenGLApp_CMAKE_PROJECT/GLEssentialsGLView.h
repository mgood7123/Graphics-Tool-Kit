//
//  MyOpenGLView.h
//  OpenGLApp
//
//  Created by Matthew Good on 28/7/21.
//
// https://developer.apple.com/library/archive/samplecode/GLEssentials/Listings/

#ifndef MyOpenGLView_h
#define MyOpenGLView_h

#import <Foundation/Foundation.h>
#import <QuartzCore/QuartzCore.h>
#import "OpenGLRenderer.h"

@interface GLEssentialsGLView : NSOpenGLView
{
    // display link for managing rendering thread
    CVDisplayLinkRef displayLink;
    OpenGLRenderer* _renderer;
    bool mouseInView;
}
@end

#endif /* MyOpenGLView_h */

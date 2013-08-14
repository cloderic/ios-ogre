//
//         DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//                     Version 2, December 2004
//
//  Copyright (C) 2013 Clodéric Mars <cloderic.mars@gmail.com>
//
//  Everyone is permitted to copy and distribute verbatim or modified
//  copies of this license document, and changing it is allowed as long
//  as the name is changed.
//
//             DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
//    TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
//
//   0. You just DO WHAT THE FUCK YOU WANT TO.

#import <OgreView.h>

#import <QuartzCore/CAEAGLLayer.h>

#include <OgreRenderWindow.h>
#include <OgreCamera.h>

@implementation OgreView

@synthesize mWindowName;
@synthesize mRenderWindow;

- (id)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code
    }
    return self;
}

+ (Class) layerClass
{
    return [CAEAGLLayer class];
}

- (void)layoutSubviews
{
    
    if(mRenderWindow != NULL)
    {
        unsigned int width = (uint)self.bounds.size.width;
        unsigned int height = (uint)self.bounds.size.height;
        
        // Resize the window
        mRenderWindow->resize(width, height);
        
        // After rotation the aspect ratio of the viewport has changed, update that as well.
        if(mRenderWindow->getNumViewports() > 0)
        {
            Ogre::Viewport *viewPort = mRenderWindow->getViewport(0);
            viewPort->getCamera()->setAspectRatio((Ogre::Real) width / (Ogre::Real) height);
        }
    }
}

@end

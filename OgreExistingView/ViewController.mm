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

#import "ViewController.h"

#import "OgreView.h"
#import "OgreApplication.h"

#import <QuartzCore/QuartzCore.h>

// private category
@interface ViewController ()
{
    OgreApplication mApplication;
    // Use of the CADisplayLink class is the preferred method for controlling your animation timing.
    // CADisplayLink will link to the main display and fire every vsync when added to a given run-loop.
    // The NSTimer class is used only as fallback when running on a pre 3.1 device where CADisplayLink
    // isn't available.
    CADisplayLink* mDisplayLink;
    NSDate *mDate;
    double mLastFrameTime;
    double mStartTime;
}
@end

@implementation ViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

- (void)startWithWindow:(UIWindow*) window
{
    UIView* view = self.view;
    unsigned int width  = view.frame.size.width;
    unsigned int height = view.frame.size.height;
    
    OgreView* ogreView = [[OgreView alloc] initWithFrame:CGRectMake(0,0,width,height)];
    
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    mLastFrameTime = 1;
    mStartTime = 0;
    
    try
    {
        mApplication.start(window, ogreView, self, width, height);
    }
    catch( Ogre::Exception& e )
    {
        std::cerr << "An exception has occurred: " <<
        e.getFullDescription().c_str() << std::endl;
    }
    
    // Call made here to override Ogre set view controller (cf. http://www.ogre3d.org/forums/viewtopic.php?f=2&t=71508&p=468814&hilit=externalviewhandle#p468814)
    window.rootViewController = self;
    
    [view addSubview:ogreView];
    
    // CADisplayLink is API new to iPhone SDK 3.1. Compiling against earlier versions will result in a warning, but can be dismissed
    // if the system version runtime check for CADisplayLink exists in -initWithCoder:. The runtime check ensures this code will
    // not be called in system versions earlier than 3.1.
    mDate = [[NSDate alloc] init];
    mLastFrameTime = -[mDate timeIntervalSinceNow];
    
    mDisplayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(update:)];
    [mDisplayLink setFrameInterval:mLastFrameTime];
    [mDisplayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
    
    [pool release];
}

- (void)stop
{
    try
    {
        mApplication.stop();
    }
    catch( Ogre::Exception& e )
    {
        std::cerr << "An exception has occurred: " <<
        e.getFullDescription().c_str() << std::endl;
    }
    
    [mDate release];
    mDate = nil;
    
    [mDisplayLink invalidate];
    mDisplayLink = nil;
}

- (void)update:(id)sender
{
    if(mApplication.isStarted())
    {
		if(mApplication.mRenderWindow->isActive())
		{
			mStartTime = mApplication.mTimer.getMillisecondsCPU();
            
			mApplication.update(mLastFrameTime);
			mApplication.draw();
            
            Camera camera;
            mApplication.pullCamera(camera);
            camera.velocity *= 0.95f;
            mApplication.pushCamera(camera);
            
            mLastFrameTime = mApplication.mTimer.getMillisecondsCPU() - mStartTime;
		}
    }
}

- (IBAction)pinch:(UIPinchGestureRecognizer *)sender
{
    Camera camera;
    mApplication.pullCamera(camera);
    camera.velocity.z = - 0.7f * sender.velocity;
    mApplication.pushCamera(camera);
}
    mApplication.pushCamera(camera);
}
@end

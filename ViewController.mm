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
#import <CoreMotion/CoreMotion.h>

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
    // CoreMotion manager (for acccelerometer/compass/gyroscope informations)
    CMMotionManager* mMotionManager;
}

@property (retain) CMAttitude* mReferenceAttitude;
@property (retain) OgreView* mOgreView;

@end

@implementation ViewController

@synthesize mOgreView;
@synthesize mReferenceAttitude;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        mReferenceAttitude = nil;
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
    unsigned int width  = self.view.frame.size.width;
    unsigned int height = self.view.frame.size.height;
    
    mOgreView = [[OgreView alloc] initWithFrame:CGRectMake(0,0,width,height)];
    [self.view addSubview:mOgreView];
    
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    mLastFrameTime = 1;
    mStartTime = 0;
    
    try
    {
        mApplication.start(window, mOgreView, width, height);
    }
    catch( Ogre::Exception& e )
    {
        std::cerr << "An exception has occurred: " <<
        e.getFullDescription().c_str() << std::endl;
    }
    
    // Linking the ogre view to the render window
    mOgreView.mRenderWindow = mApplication.mRenderWindow;
    
    // Ogre has created an EAGL2ViewController for the provided mOgreView
    // and assigned it as the root view controller of the window
    //
    // Let's first retrieve it
    UIViewController* ogreViewController = window.rootViewController;
    NSAssert(ogreViewController.view == mOgreView, @"Ogre's created view controller owns the given view.");
    NSAssert(ogreViewController.interfaceOrientation == self.interfaceOrientation, @"Ogre's view controller has the same device orientation");
    
    // I want to be the actual root view controller
    window.rootViewController = self;
    
    // but i want to add a child link with the ogre one
    [self addChildViewController:ogreViewController];
    
    // add the ogre view as a sub view and add constraints for them to match
    [self.view addSubview:mOgreView];
    [self.view sendSubviewToBack:mOgreView];
    mOgreView.translatesAutoresizingMaskIntoConstraints = NO;
    NSDictionary *views = NSDictionaryOfVariableBindings(mOgreView);
    [self.view addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|[mOgreView]|"
                                                                 options:0
                                                                 metrics:nil
                                                                   views:views]];
    [self.view addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|[mOgreView]|"
                                                                 options:0
                                                                 metrics:nil
                                                                   views:views]];
    [self.view layoutIfNeeded];
    
    // Create a CMMotionManager
    mMotionManager = [[CMMotionManager alloc] init];
    
    [self startMotionListener];
    
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
    [self stopMotionListener];
    
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
    
    [mOgreView release];
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
            
            // Update the camera
            Camera camera;
            
            mApplication.pullCamera(camera);
            camera.velocity *= 0.95f; // Decrease the linear velocity
            if (mMotionManager.deviceMotionActive)
            {
                if (self.mReferenceAttitude)
                {
                    // Computing the attitude change since last sample
                    CMAttitude* relativeAttitude = mMotionManager.deviceMotion.attitude;
                    [relativeAttitude multiplyByInverseOfAttitude: self.mReferenceAttitude];
                    switch (self.interfaceOrientation)
                    {
                        case UIInterfaceOrientationLandscapeLeft:
                            camera.pitch += Ogre::Radian(relativeAttitude.roll);
                            camera.yaw -= Ogre::Radian(relativeAttitude.pitch);
                            break;
                        case UIInterfaceOrientationLandscapeRight:
                            camera.pitch -= Ogre::Radian(relativeAttitude.roll);
                            camera.yaw += Ogre::Radian(relativeAttitude.pitch);
                            break;
                        case UIInterfaceOrientationPortraitUpsideDown:
                            camera.pitch -= Ogre::Radian(relativeAttitude.pitch);
                            camera.yaw -= Ogre::Radian(relativeAttitude.roll);
                            break;
                        case UIInterfaceOrientationPortrait:
                        default:
                            camera.pitch += Ogre::Radian(relativeAttitude.pitch);
                            camera.yaw += Ogre::Radian(relativeAttitude.roll);
                            break;
                    }
                }
                // Taking a reference attitude
                self.mReferenceAttitude = mMotionManager.deviceMotion.attitude;
            }
            
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

- (IBAction)rotate:(UIRotationGestureRecognizer *)sender
{
    Camera camera;
    mApplication.pullCamera(camera);
    camera.roll += Ogre::Radian(sender.rotation);
    sender.rotation = 0.f;
    mApplication.pushCamera(camera);
}

- (IBAction)resetCameraButton:(UIButton *)sender
{
    mApplication.resetCamera();
}

- (void)startMotionListener
{
    [mMotionManager startDeviceMotionUpdatesUsingReferenceFrame:CMAttitudeReferenceFrameXArbitraryCorrectedZVertical];
    self.mReferenceAttitude = nil;
}

- (void)stopMotionListener {
    self.mReferenceAttitude = nil;
    [mMotionManager stopDeviceMotionUpdates];
}

- (void)willRotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation duration:(NSTimeInterval)duration
{
    [self stopMotionListener];
    [super willRotateToInterfaceOrientation:toInterfaceOrientation duration:duration];
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation
{
    [super didRotateFromInterfaceOrientation:fromInterfaceOrientation];
    [self startMotionListener];
}


- (void)dealloc
{
    [self stop];
    [super dealloc];
}
@end

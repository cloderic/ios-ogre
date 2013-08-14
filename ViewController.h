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

#import <UIKit/UIKit.h>

@class OgreView;

@interface ViewController : UIViewController

- (void)startWithWindow:(UIWindow*) window;
- (void)update:(id)sender;
- (void)stop;
- (IBAction)pinch:(UIPinchGestureRecognizer *)sender;
- (IBAction)rotate:(UIRotationGestureRecognizer *)sender;
- (IBAction)resetCameraButton:(UIButton *)sender;

@end

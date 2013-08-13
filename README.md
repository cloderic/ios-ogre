# iOS - Ogre #

Sample code showing how to use Ogre3D on iOS,
- with an already existing `UIView` (allows to embed to Ogre view in any app),
- relying on Cocoa Touch for touch and accelerometer handling.

For more information, read the dedicated [blog post](http://www.crowdscontrol.net/blog/2013-08-13-a-sample-ios-application-using-ogre-3d/).

## Todo ##

- Properly handling device orientation changes.

## Building ##

1. Download and install the [iOS Ogre SDK](https://sourceforge.net/projects/ogre/files/ogre/1.8/1.8.2/OgreSDK_iOS_v1-8-2.dmg/download);
2. Dowload [iOS Ogre dependencies](http://sourceforge.net/projects/ogre/files/ogre-dependencies-mac/1.9/Ogre_iOS_6.0_Dependencies_20130424.zip/download) and unpack it at the root of where is your installed SDK (you should have a directory `iOSDependencies` in the SDK's);
3. In XCode, having opened `OgreExistingView.xcodeproj`:
	1. Open the project `OgreExistingView` settings;
	2. Go to the bottom of the *Build Settings* tab;
	3. Make sure `OGRE_SDK_ROOT` is set to the path to the Ogre iOS SDK.

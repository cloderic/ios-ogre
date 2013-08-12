# iOS - Ogre #

Sample code showing how to use Ogre3D on iOS,
- with an already existing `UIView` (allows to embed to Ogre view in any app),
- relying on Cocoa Touch for touch and accelerometer handling.

I relied mostly on Ogre's forum thread to make this work: this [one](http://www.ogre3d.org/forums/viewtopic.php?f=2&t=71508&hilit=externalviewhandle&sid=0c0d92ddd87f3f25af2ad8181a530e4f) and that [one](http://www.ogre3d.org/forums/viewtopic.php?f=21&t=71904&hilit=externalviewhandle&sid=b148dd48905674e5895284aa903a0c1b).

## Todo ##

- Properly handling device orientation changes;
- Add other *widgets* in the application to check that everything is working fine;
- Add CoreMotion based controls.

## Building ##

1. Download and install the [iOS Ogre SDK](https://sourceforge.net/projects/ogre/files/ogre/1.8/1.8.2/OgreSDK_iOS_v1-8-2.dmg/download);
2. Dowload [iOS Ogre dependencies](http://sourceforge.net/projects/ogre/files/ogre-dependencies-mac/1.9/Ogre_iOS_6.0_Dependencies_20130424.zip/download) and unpack it at the root of where is your installed SDK (you should have a directory `iOSDependencies` in the SDK's);
3.In XCode, having opened `OgreExistingView.xcodeproj`:
	1. Open the project `OgreExistingView` settings;
	2. Go to the bottom of the *Build Settings* tab;
	3. Make sure `OGRE_SDK_ROOT` is set to the path to the Ogre iOS SDK.

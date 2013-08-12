/*#include "OgreFramework.h"
#include "macUtils.h"

using namespace Ogre; 

bool OgreFramework::initOgre(Ogre::String wndTitle, void* uiWindow, void* uiView, void* uiViewController, unsigned int width, unsigned int height)
{
    new Ogre::LogManager();

	m_pLog = Ogre::LogManager::getSingleton().createLog("OgreLogfile.log", true, true, false);
	m_pLog->setDebugOutputEnabled(true);
    
    mRoot = new Ogre::Root("", Ogre::macBundlePath() + "/ogre.cfg");
    m_StaticPluginLoader.load();
    
    Ogre::NameValuePairList params;
    params["colourDepth"]          = "32";
    params["contentScalingFactor"] = 2.0;
    params["externalWindowHandle"] = Ogre::StringConverter::toString((unsigned long)uiWindow);
    params["externalViewHandle"] = Ogre::StringConverter::toString((unsigned long)uiView);
    params["externalViewController"] = Ogre::StringConverter::toString((unsigned long)uiViewController);
    
    // initialize, dont' create render window
    mRoot->initialise(false, "");
    
    // now create render window by attaching to the iOS UIView stuff
    mRenderWindow = mRoot->createRenderWindow(wndTitle,width,height,true,&params);
	mSceneManager = mRoot->createSceneManager(ST_GENERIC, "SceneManager");
	mSceneManager->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));
	
	mCamera = mSceneManager->createCamera("Camera");
	mCamera->setPosition(Vector3(0, 60, 60));
	mCamera->lookAt(Vector3(0, 0, 0));
	mCamera->setNearClipDistance(1);
    
	mViewport = mRenderWindow->addViewport(mCamera);
	mViewport->setBackgroundColour(ColourValue(0.8f, 0.7f, 0.6f, 1.0f));
    
	mCamera->setAspectRatio(Real(mViewport->getActualWidth()) / Real(mViewport->getActualHeight()));
	
	mViewport->setCamera(mCamera);
    
	Ogre::String secName, typeName, archName;
	Ogre::ConfigFile cf;
    cf.load(mResourcesRoot + "resources.cfg");
    
	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;

            // OS X does not set the working directory relative to the app,
            // In order to make things portable on OS X we need to provide
            // the loading with it's own bundle path location
            if (!Ogre::StringUtil::startsWith(archName, "/", false)) // only adjust relative dirs
                archName = Ogre::String(mResourcesRoot + archName);

            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
        }
    }
	Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    
	m_pTimer = OGRE_NEW Ogre::Timer();
	m_pTimer->reset();
    
	mRenderWindow->setActive(true);
    
	return true;
}

OgreFramework::~OgreFramework()
{
    m_StaticPluginLoader.unload();
    if(mRoot)     delete mRoot;
}
*/
/*
bool OgreFramework::touchMoved(const OIS::MultiTouchEvent &evt)
{
    OIS::MultiTouchState state = evt.state;
    int origTransX = 0, origTransY = 0;
#if !OGRE_NO_VIEWPORT_ORIENTATIONMODE
    switch(mCamera->getViewport()->getOrientationMode())
    {
        case Ogre::OR_LANDSCAPELEFT:
            origTransX = state.X.rel;
            origTransY = state.Y.rel;
            state.X.rel = -origTransY;
            state.Y.rel = origTransX;
            break;
            
        case Ogre::OR_LANDSCAPERIGHT:
            origTransX = state.X.rel;
            origTransY = state.Y.rel;
            state.X.rel = origTransY;
            state.Y.rel = origTransX;
            break;
            
            // Portrait doesn't need any change
        case Ogre::OR_PORTRAIT:
        default:
            break;
    }
#endif
	mCamera->yaw(Degree(state.X.rel * -0.1));
	mCamera->pitch(Degree(state.Y.rel * -0.1));
	
	return true;
}
 */
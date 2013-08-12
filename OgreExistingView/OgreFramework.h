//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef OGRE_FRAMEWORK_H
#define OGRE_FRAMEWORK_H

//|||||||||||||||||||||||||||||||||||||||||||||||

#include <OgreCamera.h>
#include <OgreEntity.h>
#include <OgreLogManager.h>
#include <OgreOverlay.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreRoot.h>
#include <OgreViewport.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreConfigFile.h>

#include <OISEvents.h>
#include <OISInputManager.h>
#include <OISKeyboard.h>
#include <OISMouse.h>

#define OGRE_IS_IOS 1

#   include <OISMultiTouch.h>
#define OGRE_STATIC_GLES2 1

#define USE_RTSHADER_SYSTEM

#ifdef __OBJC__
#   import <UIKit/UIKit.h>
#endif
#include "OgreStaticPluginLoader.h"

#include <SdkTrays.h>

class OgreFramework : public Ogre::Singleton<OgreFramework>, OIS::MultiTouchListener,OgreBites::SdkTrayListener
{
public:
	OgreFramework();
	~OgreFramework();
    
    bool initOgre(Ogre::String wndTitle,
                  void* uiWindow,
                  void* uiView,
                  void* uiViewController,
                  unsigned int width,
                  unsigned int height);

	void updateOgre(double timeSinceLastFrame);
	void moveCamera();
	void getInput();
    
	bool isOgreToBeShutDown()const{return m_bShutDownOgre;}
	bool touchMoved(const OIS::MultiTouchEvent &evt);
	bool touchPressed(const OIS::MultiTouchEvent &evt); 
	bool touchReleased(const OIS::MultiTouchEvent &evt);
	bool touchCancelled(const OIS::MultiTouchEvent &evt);
	
	Ogre::Root*                 m_pRoot;
	Ogre::SceneManager*			m_pSceneMgr;
	Ogre::RenderWindow*			m_pRenderWnd;
	Ogre::Camera*				m_pCamera;
	Ogre::Viewport*				m_pViewport;
	Ogre::Log*                  m_pLog;
	Ogre::Timer*				m_pTimer;
	
	OIS::InputManager*			m_pInputMgr;
	OIS::Keyboard*				m_pKeyboard;
	OIS::MultiTouch*			m_pMouse;

protected:
   // Added for Mac compatibility
   Ogre::String                 m_ResourcePath;
    
private:
	OgreFramework(const OgreFramework&);
	OgreFramework& operator= (const OgreFramework&);
    
	OgreBites::SdkTrayManager*  m_pTrayMgr;
    Ogre::FrameEvent            m_FrameEvent;
	int                         m_iNumScreenShots;
    
	bool                        m_bShutDownOgre;
	
	Ogre::Vector3				m_TranslateVector;
	Ogre::Real                  m_MoveSpeed; 
	Ogre::Degree				m_RotateSpeed; 
	float                       m_MoveScale; 
	Ogre::Degree				m_RotScale;
#ifdef OGRE_STATIC_LIB
    Ogre::StaticPluginLoader    m_StaticPluginLoader;
#endif
};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif 

//|||||||||||||||||||||||||||||||||||||||||||||||

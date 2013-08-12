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

#ifndef OGRE_DEMO_H
#define OGRE_DEMO_H

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
#include <OgreRTShaderSystem.h>

#define OGRE_STATIC_GLES2 1
#include <OgreStaticPluginLoader.h>

/** This class demonstrates basic usage of the RTShader system.
 It sub class the material manager listener class and when a target scheme callback
 is invoked with the shader generator scheme it tries to create an equivalent shader
 based technique based on the default technique of the given material.
 */
class ShaderGeneratorTechniqueResolverListener : public Ogre::MaterialManager::Listener
{
public:
    
	ShaderGeneratorTechniqueResolverListener(Ogre::RTShader::ShaderGenerator* pShaderGenerator);
    
	/** This is the hook point where shader based technique will be created.
     It will be called whenever the material manager won't find appropriate technique
     that satisfy the target scheme name. If the scheme name is out target RT Shader System
     scheme name we will try to create shader generated technique for it. 
     */
	virtual Ogre::Technique* handleSchemeNotFound(unsigned short schemeIndex, const Ogre::String& schemeName, Ogre::Material* originalMaterial, unsigned short lodIndex, const Ogre::Renderable* rend);
protected:
	Ogre::RTShader::ShaderGenerator* mShaderGenerator;			// The shader generator instance.		
};

class OgreApplication
{
public:
	OgreApplication();
	~OgreApplication();
    
	void start(void* uiWindow, void* uiView, void* uiViewController, unsigned int width, unsigned int height);
    bool isStarted();
    void stop();
    
    void update(double timeSinceLastFrame);
    void draw();
    
    Ogre::Timer mTimer;
    
    Ogre::RenderWindow* mRenderWindow;
    
private:
    OgreApplication(const OgreApplication&);
	OgreApplication& operator= (const OgreApplication&);
    
    void initializeRenderer(void* uiWindow, void* uiView, void* uiViewController, unsigned int width, unsigned int height);
    void terminateRenderer();
    
    void loadResources();
    
    void createCameraAndViewport();
    
    void createScene();
    
    bool initializeRTShaderSystem();
    void terminateRTShaderSystem();

	Ogre::SceneNode* mHeadNode;
	Ogre::Entity* mHeadEntity;
    
    Ogre::RTShader::ShaderGenerator* mShaderGenerator; // The Shader generator instance.
    ShaderGeneratorTechniqueResolverListener* mMaterialMgrListener; // Shader generator material manager listener.
    
    Ogre::Root* mRoot;
	Ogre::SceneManager* mSceneManager;
	
	Ogre::Camera* mCamera;
	Ogre::Viewport* mViewport;
	
    
    Ogre::String mResourcesRoot;
    
    Ogre::FrameEvent m_FrameEvent;
	int m_iNumScreenShots;
    
	bool m_bShutDownOgre;
	
	Ogre::Vector3 m_TranslateVector;
	Ogre::Real m_MoveSpeed;
	Ogre::Degree m_RotateSpeed;
	float m_MoveScale;
	Ogre::Degree m_RotScale;
    Ogre::StaticPluginLoader m_StaticPluginLoader;
};

#endif 

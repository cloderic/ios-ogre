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

#include "OgreApplication.h"

#include <macUtils.h>

OgreApplication::OgreApplication()
: mCameraVelocity(Ogre::Vector3::ZERO)
{
	mHeadNode		= 0;
	mHeadEntity   = 0;
    
    
	mRoot				= 0;
	mSceneManager			= 0;
	mRenderWindow        = 0;
	mCamera			= 0;
	mViewport			= 0;
    
    mResourcesRoot = Ogre::macBundlePath() + "/";
    m_FrameEvent = Ogre::FrameEvent();
}

OgreApplication::~OgreApplication()
{
    stop();
}

void OgreApplication::start(void* uiWindow, void* uiView, void* uiViewController, unsigned int width, unsigned int height)
{
    initializeRenderer(uiWindow, uiView, uiViewController, width, height);
    createCameraAndViewport();
    
    loadResources();
    
    initializeRTShaderSystem();
    
    createScene();
    
    mTimer.reset();
    
	mRenderWindow->setActive(true);
    
    mRoot->getRenderSystem()->_initRenderTargets();
    
    // Clear event times
    mRoot->clearEventTimes();
}

bool OgreApplication::isStarted()
{
    return mRoot != 0;
}

void OgreApplication::stop()
{
    terminateRTShaderSystem();
    
    terminateRenderer();
}

void OgreApplication::update(double timeSinceLastFrame)
{
    mSceneManager->setSkyBoxEnabled(true);
    
    mCamera->moveRelative(mCameraVelocity * timeSinceLastFrame);
    
	m_FrameEvent.timeSinceLastFrame = timeSinceLastFrame;
}

void OgreApplication::draw()
{
    mRoot->renderOneFrame();
}

void OgreApplication::pullCamera(Camera& camera) const
{
    camera.velocity = mCameraVelocity;
    Ogre::Matrix3 orientation;
    mCamera->getOrientation().ToRotationMatrix(orientation);
    orientation.ToEulerAnglesYXZ(camera.yaw, camera.pitch, camera.roll);
}

void OgreApplication::pushCamera(const Camera& camera)
{
    mCameraVelocity = camera.velocity;
    Ogre::Matrix3 orientation;
    orientation.FromEulerAnglesYXZ(camera.yaw, camera.pitch, camera.roll);
    mCamera->setOrientation(orientation);
}

void OgreApplication::resetCamera()
{
    mCamera->setNearClipDistance(1);
    mCamera->setPosition(Ogre::Vector3(0, 60, 60));
	mCamera->lookAt(Ogre::Vector3(0, 0, 0));
	mCamera->setAspectRatio(Ogre::Real(mViewport->getActualWidth()) / Ogre::Real(mViewport->getActualHeight()));
}

bool OgreApplication::initializeRTShaderSystem()
{
    if (Ogre::RTShader::ShaderGenerator::initialize())
    {
        mShaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
        
        mShaderGenerator->addSceneManager(mSceneManager);
        
        // Setup core libraries and shader cache path.
        Ogre::StringVector groupVector = Ogre::ResourceGroupManager::getSingleton().getResourceGroups();
        Ogre::StringVector::iterator itGroup = groupVector.begin();
        Ogre::StringVector::iterator itGroupEnd = groupVector.end();
        Ogre::String shaderCoreLibsPath;
        Ogre::String shaderCachePath;
        
        for (; itGroup != itGroupEnd; ++itGroup)
        {
            Ogre::ResourceGroupManager::LocationList resLocationsList = Ogre::ResourceGroupManager::getSingleton().getResourceLocationList(*itGroup);
            Ogre::ResourceGroupManager::LocationList::iterator it = resLocationsList.begin();
            Ogre::ResourceGroupManager::LocationList::iterator itEnd = resLocationsList.end();
            bool coreLibsFound = false;
            
            // Try to find the location of the core shader lib functions and use it
            // as shader cache path as well - this will reduce the number of generated files
            // when running from different directories.
            for (; it != itEnd; ++it)
            {
                if ((*it)->archive->getName().find("RTShaderLib") != Ogre::String::npos)
                {
                    shaderCoreLibsPath = (*it)->archive->getName() + "/";
                    shaderCachePath = shaderCoreLibsPath;
                    coreLibsFound = true;
                    break;
                }
            }
            // Core libs path found in the current group.
            if (coreLibsFound)
                break;
        }
        
        // Core shader libs not found -> shader generating will fail.
        if (shaderCoreLibsPath.empty())
            return false;
        
        // Create and register the material manager listener.
        mMaterialMgrListener = new ShaderGeneratorTechniqueResolverListener(mShaderGenerator);
        Ogre::MaterialManager::getSingleton().addListener(mMaterialMgrListener);
        
        Ogre::MaterialPtr baseWhite = Ogre::MaterialManager::getSingleton().getByName("BaseWhite", Ogre::ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME);
        baseWhite->setLightingEnabled(false);
        mShaderGenerator->createShaderBasedTechnique(
                                                     "BaseWhite",
                                                     Ogre::MaterialManager::DEFAULT_SCHEME_NAME,
                                                     Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
        mShaderGenerator->validateMaterial(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME,
                                           "BaseWhite");
        baseWhite->getTechnique(0)->getPass(0)->setVertexProgram(
                                                                 baseWhite->getTechnique(1)->getPass(0)->getVertexProgram()->getName());
        baseWhite->getTechnique(0)->getPass(0)->setFragmentProgram(
                                                                   baseWhite->getTechnique(1)->getPass(0)->getFragmentProgram()->getName());
        
        // creates shaders for base material BaseWhiteNoLighting using the RTSS
        mShaderGenerator->createShaderBasedTechnique(
                                                     "BaseWhiteNoLighting",
                                                     Ogre::MaterialManager::DEFAULT_SCHEME_NAME,
                                                     Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
        mShaderGenerator->validateMaterial(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME,
                                           "BaseWhiteNoLighting");
        Ogre::MaterialPtr baseWhiteNoLighting = Ogre::MaterialManager::getSingleton().getByName("BaseWhiteNoLighting", Ogre::ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME);
        baseWhiteNoLighting->getTechnique(0)->getPass(0)->setVertexProgram(
                                                                           baseWhiteNoLighting->getTechnique(1)->getPass(0)->getVertexProgram()->getName());
        baseWhiteNoLighting->getTechnique(0)->getPass(0)->setFragmentProgram(
                                                                             baseWhiteNoLighting->getTechnique(1)->getPass(0)->getFragmentProgram()->getName());
    }
    
    return true;
}

void OgreApplication::terminateRTShaderSystem()
{
    mShaderGenerator->removeSceneManager(mSceneManager);
    
    // Restore default scheme.
    Ogre::MaterialManager::getSingleton().setActiveScheme(Ogre::MaterialManager::DEFAULT_SCHEME_NAME);
    
    // Unregister the material manager listener.
    if (mMaterialMgrListener != NULL)
    {
        Ogre::MaterialManager::getSingleton().removeListener(mMaterialMgrListener);
        delete mMaterialMgrListener;
        mMaterialMgrListener = NULL;
    }
    
    // Finalize RTShader system.
    if (mShaderGenerator != NULL)
    {
        Ogre::RTShader::ShaderGenerator::finalize();
        mShaderGenerator = NULL;
    }
}

void OgreApplication::initializeRenderer(void* uiWindow, void* uiView, void* uiViewController, unsigned int width, unsigned int height)
{
    OgreAssert(!mRoot, "Existing root");
    OgreAssert(!mRenderWindow, "Existing render window");
    OgreAssert(!mSceneManager, "Existing scene manager");
    
    mRoot = new Ogre::Root("", mResourcesRoot + "ogre.cfg");
    m_StaticPluginLoader.load();
    
    Ogre::NameValuePairList params;
    params["colourDepth"] = "32";
    params["contentScalingFactor"] = 2.0;
    params["externalWindowHandle"] = Ogre::StringConverter::toString((unsigned long)uiWindow);
    params["externalViewHandle"] = Ogre::StringConverter::toString((unsigned long)uiView);
    params["externalViewController"] = Ogre::StringConverter::toString((unsigned long)uiViewController);
    
    // Initialize w/o creating a renderwindow.
    mRoot->initialise(false, "");
    
    // Create the window and attach it to the given UI stuffs.
    mRenderWindow = mRoot->createRenderWindow("",width,height,true,&params);
    
    mSceneManager = mRoot->createSceneManager(Ogre::ST_GENERIC, "SceneManager");
}

void OgreApplication::terminateRenderer()
{
    mRoot->queueEndRendering();
    
    m_StaticPluginLoader.unload();
    
    delete mRoot;
    mRoot = 0;
    
    mRenderWindow = 0;
    mSceneManager = 0;
}

void OgreApplication::loadResources()
{
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

}

void OgreApplication::createCameraAndViewport()
{
    OgreAssert(mSceneManager, "NULL scene manager");
    OgreAssert(!mCamera, "Existing camera");
    OgreAssert(!mViewport, "Existing viewport");
	
	mCamera = mSceneManager->createCamera("Camera");

	mViewport = mRenderWindow->addViewport(mCamera);
	mViewport->setBackgroundColour(Ogre::ColourValue(0.8f, 0.7f, 0.6f, 1.0f));
    
    resetCamera();
	
	mViewport->setCamera(mCamera);

}

void OgreApplication::createScene()
{
    mSceneManager->setAmbientLight(Ogre::ColourValue(0.7f, 0.7f, 0.7f));
	mSceneManager->setSkyBox(true, "Examples/SpaceSkyBox");
    
	mSceneManager->createLight("Light")->setPosition(75,75,75);
    
	mHeadEntity = mSceneManager->createEntity("Head", "ogrehead.mesh");
	mHeadNode = mSceneManager->getRootSceneNode()->createChildSceneNode("HeadNode");
	mHeadNode->attachObject(mHeadEntity);
}

ShaderGeneratorTechniqueResolverListener::ShaderGeneratorTechniqueResolverListener(Ogre::RTShader::ShaderGenerator* pShaderGenerator)
{
    mShaderGenerator = pShaderGenerator;
}

Ogre::Technique* ShaderGeneratorTechniqueResolverListener::handleSchemeNotFound(unsigned short schemeIndex, const Ogre::String& schemeName, Ogre::Material* originalMaterial, unsigned short lodIndex, const Ogre::Renderable* rend)
{
    Ogre::Technique* generatedTech = NULL;
    
    // Case this is the default shader generator scheme.
    if (schemeName == Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME)
    {
        bool techniqueCreated;
        
        // Create shader generated technique for this material.
        techniqueCreated = mShaderGenerator->createShaderBasedTechnique(
                                                                        originalMaterial->getName(),
                                                                        Ogre::MaterialManager::DEFAULT_SCHEME_NAME,
                                                                        schemeName);
        
        // Case technique registration succeeded.
        if (techniqueCreated)
        {
            // Force creating the shaders for the generated technique.
            mShaderGenerator->validateMaterial(schemeName, originalMaterial->getName());
            
            // Grab the generated technique.
            Ogre::Material::TechniqueIterator itTech = originalMaterial->getTechniqueIterator();
            
            while (itTech.hasMoreElements())
            {
                Ogre::Technique* curTech = itTech.getNext();
                
                if (curTech->getSchemeName() == schemeName)
                {
                    generatedTech = curTech;
                    break;
                }
            }				
        }
    }
    
    return generatedTech;
}


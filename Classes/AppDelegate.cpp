#include "AppDelegate.h"
#include "cocos2d.h"
#include "Level.h"
#include "CCEGLView.h"
#include "SimpleAudioEngine.h"


AppDelegate::AppDelegate() {}

AppDelegate::~AppDelegate()
{
    CocosDenshion::SimpleAudioEngine::end();
}

bool AppDelegate::initInstance()
{
    bool bRet = false;
    do 
    {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)

		// Initialize OpenGLView instance, that release by CCDirector when application terminate.
		// The HelloWorld is designed as HVGA.
		cocos2d::CCEGLView * pMainWnd = new cocos2d::CCEGLView();
		const int IPHONE_WIDTH = 480;
		const int IPHONE_HEIGHT = 320;
		CC_BREAK_IF(! pMainWnd
				|| ! pMainWnd->Create(TEXT("Hello Mr. Death: the Game"), IPHONE_WIDTH, IPHONE_HEIGHT));

#endif  // CC_PLATFORM_WIN32

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

        // OpenGLView initialized in testsAppDelegate.mm on ios platform, nothing need to do here.

#endif  // CC_PLATFORM_IOS

		bRet = true;
	} while (0);
	return bRet;
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    cocos2d::CCDirector *pDirector = cocos2d::CCDirector::sharedDirector();
    pDirector->setOpenGLView(&cocos2d::CCEGLView::sharedOpenGLView());

	// enable High Resource Mode(2x, such as iphone4) and maintains low resource on other devices.
//     pDirector->enableRetinaDisplay(true);

    // turn on display FPS
    pDirector->setDisplayFPS(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    cocos2d::CCScene *pScene = Level::scene();

    // run
    pDirector->runWithScene(pScene);
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    cocos2d::CCDirector::sharedDirector()->pause();

    CocosDenshion::SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    cocos2d::CCDirector::sharedDirector()->resume();

    CocosDenshion::SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

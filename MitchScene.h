#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include <CCMutableDictionary.h>

#include "Box2D/Box2D.h"

#include "SimpleAudioEngine.h"

#include <iostream>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <windows.h>
#endif

class HelloWorld : public cocos2d::CCLayer
{
public:
	HelloWorld();
	~HelloWorld();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
    virtual void menuCloseCallback(CCObject* pSender);

    // implement the "static node()" method manually
    LAYER_NODE_FUNC(HelloWorld);

	void gameLoop(float dt);
	void update(float dt);
	void checkKeyboardInput(float dt);

private:
	/**
		sets the camera to the centre of the screen when Mr. Death is in the first 1/3
		of the window.  Only works when he goes forward.
	*/
	void setViewpointCentre();

	/**
	 * Adds a target to the scene for Mr. Death to attack.
	 */
	void addTarget();
	void spriteMoveFinished(CCNode* sender);
	void ccTouchesBegan(cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent);
	void ccTouchesEnded(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);
	void deathDidFinishMoving(CCNode* sender);
	/**
	* shows the sprite of mr death attacking
	*/
	void showMrDeathAttacking();

	/**
	 * reverts the mr. death sprite back to normal
	 */
	void showMrDeathNormal();

	void moveMrDeath(bool forward);
	void mrDeathJump();

	/**
	 * makes the first sprite invisible and the second sprite visible
	 */
	void swapSprites(cocos2d::CCSprite* spriteToMakeInvisible, cocos2d::CCSprite* spriteToMakeVisible);

	static const int ATTACK_TAG = 0;
	static const int WATERMELON_TAG = 1;
	static const int BG_TAG = 2;
	static const int MENU_TAG = 3;

	cocos2d::CCSprite* mrDeathSprite;
	cocos2d::CCSprite* mrDeathAttackingSprite;
	cocos2d::CCSprite* blobSprite;
	bool isTouchingMrDeath;
	bool isMrDeathAttacking;
	bool isMrDeathMoving;
	bool isMrDeathFacingRight;

	// tiled map editor test
	cocos2d::CCTMXTiledMap* tileMap;
	cocos2d::CCTMXLayer* tiledBg;

protected:
	cocos2d::CCMutableArray<cocos2d::CCSprite*> *_watermelons;
};

#endif  // __HELLOWORLD_SCENE_H__
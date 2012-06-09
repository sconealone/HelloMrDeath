#pragma once
#ifndef LEVEL_H
#define LEVEL_H
#define TIMESTEP 1.0f/60.0f

#include "cocos2d.h"
#include "Box2D.h"
#include "MrDeath.h"
#include "MDUtil.h"

class Character;
class MrDeath;

class Level: public cocos2d::CCLayer {
public:
	Level();
	~Level();
	
	/**
	 * Initializes the level
	 * creating the floor, platforms, background,
	 * enemies, and Mr. Death
	 */
	virtual bool init();

	CCLayer* getGameLayer() {return gameLayer;}

	/**
	 * Returns the scene.  The Level is added
	 * to the scene.
	 */
	static cocos2d::CCScene* scene();

	/**
	 * Macro needed by the scene function
	 */ 
	LAYER_NODE_FUNC(Level);

	/**
	 * Updates the level every tick.  Things that
	 * can go here include stepping the world,
	 * updating the enemy AI, checking for input
	 */
	virtual void update(float dt);
	
	virtual void ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent);

	virtual void ccTouchesEnded(cocos2d::CCSet* pTouches, cocos2d::CCEvent* pEvent);
	
	void passingAction();
	
	/**
	 * The physics world
	 */
	CC_SYNTHESIZE(b2World*,world,World);



	// Tags
	static const int TRANSITION_TAG = 100;

private:
	// functions

	/**
	 * Checks for input like button presses
	 */
	void checkInput();

	#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	/**
	Checks the keyboard for key presses, and
	translates them into game commands
	*/
	void checkKeyboard();
	#endif

	/**
	 * Centres the camera (the game layer)
	 */
	void centreCamera();

	/**
	 * Initializes the platforms from a tiledmap and adds them to the world
	 */ 
	void initPlatformsFromTiledMap();
	

	/**
	 * Creates the box2d body for a platform and adds
	 * it to the world.
	 */
	void createPlatformBody(float width, float height, float centerX, float centerY);

	
	/**
	Initializes the buttons that will be used to
	control the game.
	*/
	void initButtons();

	/**
	Initializes the physics world.
	*/
	void initWorld();
	void initBg();
	void initWorldBorders();
	void initPlaceHolderWorldBorders();
	void initWeather();
	/**
	Initializes the player character.
	*/
	void initPC();

	void attack(cocos2d::CCObject* sender);
	void specialAttack(cocos2d::CCObject* sender);
	void jump(cocos2d::CCObject* sender);

	/**
	 A set of boolean to check whether the touch is on the buttons
	 **/
	
	bool isRightArrow(float x, float y);
	bool isRightArrow(cocos2d::CCPoint& point);
	bool isLeftArrow(float x, float y);
	bool isLeftArrow(cocos2d::CCPoint& point);

	// fields
	typedef cocos2d::CCLayer super;
	/**
	 * The game layer is the layer where we will add our background
	 * and characters
	 */
	cocos2d::CCLayer* gameLayer;

	/**
	 * This is the layer where we put our menu and buttons
	 */
	cocos2d::CCLayer* controlLayer;

	/**
	 * Holds all the enemies that are on the screen
	 */
	cocos2d::CCMutableArray<Character*>* enemies;


	/**
	 * Mr. Death!
	 */
	MrDeath* death;

	
	cocos2d::CCTMXLayer* platformsLayer;

	/**
	 * The tiled map should be initialized in init
	 */
	cocos2d::CCTMXTiledMap *tiledMap;

	/**
	 * A meta layer for determining if things are platforms
	 */
	cocos2d::CCTMXLayer* collidableLayer;

	// Control buttons
	cocos2d::CCSprite *leftbutton;
	cocos2d::CCSprite *rightbutton;
	cocos2d::CCMenuItemImage *attackbutton;
	cocos2d::CCMenuItemImage *jumpbutton;
	cocos2d::CCMenuItemImage *specialbutton;
	
	/**
	 * A boolean for whether the player is touching
	 **/
	bool isTouching;


};

#endif // define LEVEL_H
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


	
	/**
	 * The physics world
	 */
	CC_SYNTHESIZE(b2World*,world,World);

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
	#else
	/**
	Checks for button touches and translates
	them into game commands
	*/
	void checkTouches();
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
	 * True if there is a platform at this tile coordinate
	 * Checks if the GID is non-zero at that tile coord
	 * A 0 GID indicates an empty tile
	 * Assumes only platform tiles are in the platform tile layer
	 */
	bool isPlatform(cocos2d::CCPoint tileCoord);

	/**
	 * Creates the box2d body for a platform and adds
	 * it to the world.
	 */
	void createPlatformBody(float width, float height, float centerX, float centerY);


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

	
	cocos2d::CCTMXLayer* backgroundLayer;

	/**
	 * The tiled map should be initialized in init
	 */
	cocos2d::CCTMXTiledMap *tiledMap;

	/**
	 * A meta layer for determining if things are platforms
	 */
	cocos2d::CCTMXLayer* platformLayer;

	/**
	Initializes the physics world.
	*/
	void initWorld();

	void initWorldBorders();

	/**
	Initializes the player character.
	*/
	void initPC();
};

#endif // define LEVEL_H
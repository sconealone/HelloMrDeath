#pragma once
#ifndef LEVEL_H
#define LEVEL_H
#define TIMESTEP 1.0f/60.0f

#include "cocos2d.h"
#include "Box2D.h"
#include "Character.h"
#include "MrDeath.h"
#include "Enemy.h"
class Enemy;
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

private:
	// functions

	/**
	 * Checks for input like button presses
	 */
	void checkInput();

	/**
	 * Centres the camera (the game layer)
	 */
	void centreCamera();

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
	cocos2d::CCMutableArray<Enemy*>* enemies;

	/**
	 * The physics world
	 */
	b2World* world;

	/**
	 * Mr. Death!
	 */
	MrDeath* death;


};

#endif // define LEVEL_H
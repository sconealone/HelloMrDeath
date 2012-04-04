#pragma once
#ifndef LEVEL_H
#define LEVEL_H

#include "cocos2d.h"
#include "Box2D.h"
#include "Character.h"
#include "MrDeath.h"
#include "Enemy.h"

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

	// fields


}

#endif // define LEVEL_H
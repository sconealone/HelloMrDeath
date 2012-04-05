#ifndef CHARACTER_H
#define CHARACTER_H

#include "cocos2d.h"
#include <CCMutableDictionary.h>

#include "Box2D.h"

#include "SimpleAudioEngine.h"

#include <iostream>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <windows.h>
#endif

class Character  
{
public:

	// Attack value for each character
	static const int attackvalue;

	// HP value for each character
	int hpvalue;

	// Move left function for characters
	virtual void moveLeft();

	// Move right function for characters
	virtual void moveRight();

	// Attack function for characters
	void attack();



};

#endif
/*
 *  MrDeath.cpp
 *  HelloMrDeath
 *
 *  Created by Olivia Zhang on 12-04-28.
 *  Copyright 2012 UBC. All rights reserved.
 *
 */
#include "MrDeath.h"

using namespace cocos2d;




void MrDeath::jump(){
	
}

void MrDeath::attack() {
}

void MrDeath::moveLeft() {
}

void MrDeath::moveRight() {
}

void MrDeath::initActions2() {
	
	// TODO: placeholder
	CCAction* action = initAction("death", 4, true);
	sprite->runAction(action);

}


Character* MrDeath::initCharacterWithNameInWorld(Character* myChar, string name, b2World* world) {
	Character::initCharacterWithNameInWorld(myChar, name, world);
	((MrDeath*)myChar)->initActions2();
	return myChar;
}
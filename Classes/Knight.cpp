#include "Knight.h"
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;
using namespace cocos2d;

Knight::Knight(Level* level) : Character() {
	this->level = level;
	hpValue = 3;
	speed = 4.0f;
	isFacingRight = false;
}

Knight::~Knight() {
	CC_SAFE_RELEASE_NULL(standStillAnimation);
	CC_SAFE_RELEASE_NULL(attackAnimation);
}



void Knight::attack(){
	cout << "attacking";
	
	//need to be replaced with a walk animation later
	CCFiniteTimeAction *moveleft = CCMoveTo::actionWithDuration(2.0f, ccp(this->getPosition().x-10,this->getPosition().y));
	CCFiniteTimeAction *moveright = CCMoveTo::actionWithDuration(2.0f, ccp(this->getPosition().x+10,this->getPosition().y));
	
	//attackAction = initAction(attackAnimation,false);
//
//
//	CCFiniteTimeAction *resumeAction = initAction(standStillAnimation,false);
//	
//	sprite->runAction(CCSequence::actions(moveleft,attackAction,resumeAction,NULL));
//	
//	attackAction->release();
	
	this->moveLeft(5.0f);

}

void Knight::initAnimations(){
	standStillAnimation = initAnimation("knight",1);
	attackAnimation = initAnimation("knight",2,3);
	
}

bool Knight::checkDeathDistance(Level* level){
	MrDeath* death = level->getDeath();
	float death_x = death->getPosition().x;
	float knight_x = this->getPosition().x;
	return (death_x - knight_x > -3 && death_x - knight_x < 3);
}

void Knight::update(){

	Character::update();

	b2Vec2 vec = body->GetPosition();
	setPosition(vec);
	bool distance = checkDeathDistance(level);
	if (distance) {
		this->attack();
	}
	

}



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
	boundingBox = CCRect(0.0f, 0.0f, 32.0f, 55.0f);
	height = MDUtil::pixelsToMetres(boundingBox.size.height);
	width = MDUtil::pixelsToMetres(boundingBox.size.width);
	sprite_body_offset = 14;
	coolDownCount = 30;
}

Knight::~Knight() {
	CC_SAFE_RELEASE_NULL(standStillAnimation);
	CC_SAFE_RELEASE_NULL(attackAnimation);
}


void Knight::moveLeft(float vel) {
	CCFiniteTimeAction* flip = NULL;
	if (isFacingRight) { 
		flip = CCFlipX::actionWithFlipX(false);
		sprite->runAction(flip);
	}
	isFacingRight = false;
	body->SetLinearVelocity(b2Vec2(vel*-1,body->GetLinearVelocity().y));
}



void Knight::moveRight(float vel) {
	CCFiniteTimeAction* flip = NULL;
	if (!isFacingRight) {
		flip = CCFlipX::actionWithFlipX(true);
		sprite->runAction(flip);
	}
	isFacingRight = true;
	body->SetLinearVelocity(b2Vec2(vel,body->GetLinearVelocity().y));
}

void Knight::attack(){
	
	attackAction = initAction(attackAnimation,false);

	CCFiniteTimeAction *resumeAction = initAction(standStillAnimation,false);
	
	int dis = (int)checkDeathDistance(level);
	
	if (dis > -4 && dis < -1) {
		this->moveLeft(5.0f);
	}
	else if (dis < 4 && dis > 1){
		this->moveRight(5.0f);
	}
	else if (dis <= 1 && dis >= -1){
		cout << coolDownCount;
		if (coolDownCount == 0) {
			sprite->runAction(CCSequence::actions(attackAction, resumeAction, NULL));
		} 
	} else {
		this->stopMoving();
	}


	
	attackAction->release();

}

void Knight::initAnimations(){
	standStillAnimation = initAnimation("knight",1);
	attackAnimation = initAnimation("knight",2,3);
	
}

float Knight::checkDeathDistance(Level* level){
	MrDeath* death = level->getDeath();
	float death_x = death->getPosition().x;
	float knight_x = this->getPosition().x;
	
	return death_x - knight_x;

}

void Knight::setPosition(b2Vec2& pos){
	Character::setPosition(pos);
	setPos();
}

void Knight::setPosition(CCPoint pos){
	Character::setPosition(pos);
	setPos();
}

void Knight::update(){

	Character::update();
	this->attack();
	if (coolDownCount > 0) {
		coolDownCount--;
	}else {
		coolDownCount = 30;
	}

	
	
}


void Knight::initFixtureDef(b2FixtureDef *fixDef, b2PolygonShape *shape) {
	Character::initFixtureDef(fixDef, shape);
	fixDef->density = 1.0f;
}

void Knight::setPos(){
	CCPoint offsetpos = ccpAdd(sprite->getPosition(), ccp(0.0f, this->sprite_body_offset));
	sprite->setPosition(offsetpos);
	boundingBox.origin = offsetpos;
}




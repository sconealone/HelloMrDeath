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

MrDeath::MrDeath(cocos2d::CCLayer* layer) : Character(){
	this->layer = layer;
	isAttacking = false;
	isMoving = false;
	isJumping = false;
	attackStartupAnimation = NULL;
	jumpAnimation = NULL;
}

MrDeath::~MrDeath() {
	CC_SAFE_RELEASE_NULL(standStillAnimation);
	CC_SAFE_RELEASE_NULL(attackStartupAnimation);
	CC_SAFE_RELEASE_NULL(attackAnimation);
}

void MrDeath::jump(){
	
}

void MrDeath::attack() {
	if (!isAttacking) {
		CCFiniteTimeAction* attackStartAction = initAction(attackStartupAnimation, false);
		attackAction = initAction(attackAnimation, false);
		CCFiniteTimeAction* attackBegin = CCCallFuncN::actionWithTarget(layer, callfuncN_selector(MrDeath::attackStart));
		CCFiniteTimeAction* attackDone = CCCallFuncN::actionWithTarget(layer,callfuncN_selector(MrDeath::attackStop));
		sprite->runAction(CCSequence::actions(attackStartAction, attackBegin, attackAction, attackDone, NULL));
		attackAction->release();
		attackStartAction->release();
	}
}

void MrDeath::attackStart(CCNode* sender) {
	isAttacking = true;
}

void MrDeath::attackStop(CCNode* sender) {
	isAttacking = false;
	stopMoving();
}


void MrDeath::moveLeft() {
}

void MrDeath::moveRight() {
}

void MrDeath::stopMoving() {
	sprite->runAction(CCAnimate::actionWithAnimation(standStillAnimation, true));
}

void MrDeath::initAnimations() {
	attackStartupAnimation = initAnimation("death", 1, 2);
	attackAnimation = initAnimation("death", 2, 3);
	standStillAnimation = initAnimation("death", 1);
}

CCPoint b2VecToCCPoint(b2Vec2 vec) {
	return ccp(vec.x, vec.y);
}

void MrDeath::update() {
	setPosition(b2VecToCCPoint(body->GetPosition()));
	checkCollisions();
}

void MrDeath::checkCollisions() {
}
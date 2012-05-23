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

MrDeath::MrDeath(cocos2d::CCLayer* level) : Character(){
	this->level = level;
	isAttacking = false;
	isMoving = false;
	isJumping = false;
}

MrDeath::~MrDeath() {
	attackAnimation->release();
}

void MrDeath::jump(){
	
}

void MrDeath::attack() {
	if (!isAttacking) {
		isAttacking = true;
		CCFiniteTimeAction* attackAction = initAction(attackAnimation, false);
		CCFiniteTimeAction* attackDone = CCCallFuncN::actionWithTarget(level,callfuncN_selector(MrDeath::attackStop));
		//sprite->runAction(CCSequence::actions(attackAction, attackDone, NULL)); // this line causes the program to crash, the error is %esp is not saved
		sprite->runAction(attackAction);
		attackAction->release();
		isAttacking = false;
	}
}


void MrDeath::attackStop() {
	isAttacking = false;
	// resume previous animation
}


void MrDeath::moveLeft() {
}

void MrDeath::moveRight() {
}

void MrDeath::stopMoving() {
}

void MrDeath::initAnimations() {
	attackAnimation = initAnimation("death", 4);
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
/*
 *  MrDeath.cpp
 *  HelloMrDeath
 *
 *  Created by Olivia Zhang on 12-04-28.
 *  Copyright 2012 UBC. All rights reserved.
 *
 */
#include "MrDeath.h"
#include <stdlib.h>
#include <iostream>

using namespace std;

using namespace cocos2d;

MrDeath::MrDeath(cocos2d::CCLayer* layer) : Character(){
	this->layer = layer;
	isAttacking = false;
	isMoving = false;
	isJumping = false;
	attackStartupAnimation = NULL;
	jumpAnimation = NULL;
	soulGauge = 0;
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
		
		sprite->runAction(CCSequence::actions(attackStartAction, attackBegin, attackAction, attackDone, CCAnimate::actionWithAnimation(standStillAnimation, false),NULL));
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

void MrDeath::specialAttack(){
	
	cout << "special attack!" << endl;
	
	CCParticleSystem* soultest;
	soultest = CCParticleExplosion::node();
	soultest->setEmitterMode(kCCParticleModeRadius);
	soultest->setTexture(CCTextureCache::sharedTextureCache()->addImage("soul.png"));
	soultest->setAngleVar(360.0f);
	soultest->setLife(2.0f);
	soultest->setStartRadius(sprite->getContentSize().height/2);
	soultest->setEndRadius(3*sprite->getContentSize().height);
	ccColor4F color;
	color.r = 0.0f;
	color.b = 0xD;
	color.g = 0.0f;
	color.a = 1.0f;

	ccColor4F endColor;
	color.r = 0x0;
	color.b = 0xB;
	color.a = 0.0f;
	color.a = 1.0f;
	soultest->setStartColor(color);
	soultest->setEndColor(endColor);
	soultest->setPosition(MDUtil::toCCPoint(position));
	layer->addChild(soultest);
}


void MrDeath::moveLeft() {
}

void MrDeath::moveRight() {
}

void MrDeath::stopMoving() {
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
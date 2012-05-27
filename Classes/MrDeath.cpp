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
	specialAttackParticles = CCParticleExplosion::node();
	specialAttackParticles->setEmitterMode(kCCParticleModeRadius);
	specialAttackParticles->setTexture(CCTextureCache::sharedTextureCache()->addImage("soul.png"));
	specialAttackParticles->setAngleVar(360.0f);
	specialAttackParticles->setLife(2.0f);
	specialAttackParticles->setStartRadius(sprite->getContentSize().height/2);
	specialAttackParticles->setEndRadius(3*sprite->getContentSize().height);
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
	specialAttackParticles->setStartColor(color);
	specialAttackParticles->setEndColor(endColor);
	specialAttackParticles->setPosition(MDUtil::toCCPoint(position));
	layer->addChild(specialAttackParticles);
}


void MrDeath::moveLeft() {
	CCFiniteTimeAction* flip = NULL;
	if (isFacingRight) { 
		flip = CCFlipX::actionWithFlipX(true);
		sprite->runAction(flip);
	}
	isFacingRight = false;
	body->SetLinearVelocity(b2Vec2(MR_DEATH_SPEED,body->GetLinearVelocity().y));
}

void MrDeath::moveRight() {
	CCFiniteTimeAction* flip = NULL;
	if (!isFacingRight) {
		flip = CCFlipX::actionWithFlipX(false);
		sprite->runAction(flip);
	}
	isFacingRight = true;
	body->SetLinearVelocity(b2Vec2(MR_DEATH_SPEED,body->GetLinearVelocity().y));
}

void MrDeath::stopMoving() {
	body->SetLinearVelocity(b2Vec2(0.0f, body->GetLinearVelocity().y));
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



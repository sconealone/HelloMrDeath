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
	previousYVelocity = 0.0f;
	wasAccelerating = false;
	speed = MR_DEATH_SPEED;
	boundingBox = CCRect(0.0f, 0.0f, 32.0f, 55.0f);
	height = MDUtil::pixelsToMetres(boundingBox.size.height);
	width = MDUtil::pixelsToMetres(boundingBox.size.width);
	jumpSpeed = 40.0f;
	hpValue = 5;
}

MrDeath::~MrDeath() {
	CC_SAFE_RELEASE_NULL(standStillAnimation);
	CC_SAFE_RELEASE_NULL(attackStartupAnimation);
	CC_SAFE_RELEASE_NULL(attackAnimation);
}

void MrDeath::jump(){
	if (!isJumping) {
		isJumping = true;
		body->ApplyLinearImpulse(b2Vec2(0.0f, jumpSpeed), body->GetPosition()); 
	}
}

void MrDeath::checkIfLanded() {
	float currentYVelocity = body->GetLinearVelocity().y;
	bool isAccelerating = currentYVelocity < previousYVelocity;
	if (isAccelerating) {
		isJumping = true;
	}
	if (isJumping) {
		previousYVelocity = currentYVelocity;
		if (wasAccelerating && !isAccelerating) {
			isJumping = false;
			wasAccelerating = false;
		} 
		else if (isAccelerating) {
			wasAccelerating = true;
		}
	}
}

void MrDeath::attack() {
	if (!isAttacking) {
		isAttacking = true;
		CCFiniteTimeAction* attackStartAction = initAction(attackStartupAnimation, false);
		attackAction = initAction(attackAnimation, false);
		CCFiniteTimeAction* attackBegin = CCCallFuncN::actionWithTarget(layer, callfuncN_selector(MrDeath::attackStart));
		CCFiniteTimeAction* attackDone = CCCallFuncN::actionWithTarget(layer,callfuncN_selector(MrDeath::attackStop));

		// TODO: Once there is a seperate jumping/running animation, this needs to choose between them
		CCFiniteTimeAction *resumePreviousAction = CCAnimate::actionWithAnimation(standStillAnimation, false);

		sprite->runAction(CCSequence::actions(attackStartAction, attackAction, resumePreviousAction, NULL));
		attackAction->release();
		attackStartAction->release();

		isAttacking = false;
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
	const int EXPLOSION_TAG = 0xBADBEEF;
	specialAttackParticles = CCParticleExplosion::node();
	if (layer->getChildByTag(EXPLOSION_TAG)) {
		layer->removeChildByTag(EXPLOSION_TAG, true);
	}
	specialAttackParticles->setPosition(sprite->getPosition());
	specialAttackParticles->setTag(EXPLOSION_TAG);
	specialAttackParticles->setEmitterMode(kCCParticleModeRadius);
	specialAttackParticles->setTexture(CCTextureCache::sharedTextureCache()->addImage("soul.png"));
	specialAttackParticles->setLife(1.0f);
	specialAttackParticles->setStartRadius(sprite->getContentSize().height/2);
	specialAttackParticles->setEndRadius(3*sprite->getContentSize().height);
	specialAttackParticles->setStartSize(32.0f);
	specialAttackParticles->setTotalParticles(50);
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



void MrDeath::initAnimations() {
	attackStartupAnimation = initAnimation("death", 1, 2);
	attackAnimation = initAnimation("death", 2, 3);
	standStillAnimation = initAnimation("death", 1);

}

CCPoint b2VecToCCPoint(b2Vec2 vec) {
	return ccp(vec.x, vec.y);
}

void MrDeath::update() {
	Character::update();
	checkIfLanded();
	checkCollisions();
}

void MrDeath::checkCollisions() {
}

void MrDeath::initFixtureDef(b2FixtureDef *fixDef, b2PolygonShape *shape) {
	Character::initFixtureDef(fixDef, shape);
	fixDef->friction = 0.0f;
}




void MrDeath::setPosition(b2Vec2& pos) {
	Character::setPosition(pos);
	setPosition();
}

void MrDeath::setPosition(cocos2d::CCPoint pos) {
	Character::setPosition(pos);
	setPosition();
}

void MrDeath::setPosition() {
	CCPoint offsettedPos = ccpAdd(sprite->getPosition(), ccp(0.0f, (float)SPRITE_BODY_OFFSET));
	sprite->setPosition(offsettedPos);
	boundingBox.origin = offsettedPos;
}

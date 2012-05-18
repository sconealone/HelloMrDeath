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

CCSprite *death;


MrDeath::MrDeath(){
	
	
	death = CCSprite::spriteWithSpriteFrameName("death1.png");
	
	death-> setPosition(ccp(200,200));
	
}




//cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("death.plist");

void MrDeath::attackAnimation(){
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("death.plist");
	CCSpriteBatchNode *deathSpritSheet = cocos2d::CCSpriteBatchNode::spriteSheetWithFile("death.png");
	
	CCMutableArray<CCSpriteFrame *> *deathAnimFrames = new CCMutableArray<cocos2d::CCSpriteFrame *>;
	
	deathAnimFrames->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("death1.png"));
	deathAnimFrames->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("death2.png"));
	deathAnimFrames->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("death3.png"));
	deathAnimFrames->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("death4.png"));
	deathAnimFrames->addObject(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("death1.png"));
	
	CCAnimation *attackAnim = CCAnimation::animationWithFrames(deathAnimFrames);
	
	CCAction *attackAction = CCAnimate::actionWithAnimation(attackAnim, false);
	
	death->runAction(attackAction);
	
	deathSpritSheet-> addChild(death, 1);
	
}

void MrDeath::jump(){
	
}

void MrDeath::specialAttack(){
}


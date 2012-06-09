#include "Knight.h"
using namespace cocos2d;

Knight::Knight(cocos2d::CCLayer* layer) : Character() {
	hpValue = 10;
	isFacingRight = false;
	isNearDeath = false;
}



Knight::~Knight() {

}

void Knight::attack() {
}

// TODO: Placeholder until the .plist is finished
Knight* Knight::initKnightInWorldAtPosition(Knight* knight, b2World* world, b2Vec2& position) {
	knight->sprite = CCSprite::spriteWithFile("knight1.png");
	knight->world = world;
	knight->initBody();
	knight->setPosition(position);
	return knight;
}



void Knight::update(float dt) {

}
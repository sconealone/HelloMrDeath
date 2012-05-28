#include "Knight.h"
using namespace cocos2d;

Knight::Knight() : Enemy() {
	isFacingRight = false;
}



Knight::~Knight() {

}



// TODO: Placeholder until the .plist is finished
Knight* Knight::initKnightInWorldAtPosition(Knight* knight, b2World* world, b2Vec2& position) {
	knight->sprite = CCSprite::spriteWithFile("death1.png");
	knight->world = world;
	knight->initBody();
	knight->setPosition(position);
	return knight;
}



void Knight::update(float dt) {

}
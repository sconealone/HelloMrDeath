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
}

Knight::~Knight() {
}

void Knight::attack(){
	attackAction = initAction(attackAnimation,false);
	
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
	checkDeathDistance(level);
}



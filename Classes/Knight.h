#pragma once
#ifndef KNIGHT_H
#define KNIGHT_H
#include "Character.h"
#include "Level.h"
class Character;
class Level;

class Knight : public Character {
public:
	Knight(Level* level);
	
	bool checkDeathDistance(Level* level);
	
	virtual ~Knight();
	
	virtual void update();
	
	virtual void attack();
	
	

private:
	Level* level;
	
	virtual void initAnimations();
	
	/**
	 *The animations for the knight's movement
	 **/
	//note: we need animation for the knight's walking
	cocos2d::CCAnimation* walkAnimation;
	cocos2d::CCAnimation* standStillAnimation;
	
	
};

#endif
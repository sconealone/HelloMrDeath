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

	
	float checkDeathDistance(Level* level);
	
	virtual ~Knight();
	
	virtual void update();
	
	virtual void attack();
	
	virtual void moveLeft(float vel);
	
	virtual void moveRight(float vel);
	
	virtual void setPosition(b2Vec2& pos);
	
	virtual void setPosition(cocos2d::CCPoint pos);
	
protected:
	virtual void initFixtureDef(b2FixtureDef* fixDef, b2PolygonShape *shape);

private:
	Level* level;
	
	virtual void initAnimations();
	
	void setPos();
	
	/**
	 *The animations for the knight's movement
	 **/
	//note: we need animation for the knight's walking
	cocos2d::CCAnimation* walkAnimation;
	cocos2d::CCAnimation* standStillAnimation;
	
};

#endif
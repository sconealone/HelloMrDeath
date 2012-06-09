#pragma once
#ifndef KNIGHT_H
#define KNIGHT_H
#include "Character.h"
class Character;
class CCLayer;

class Knight : public Character {
	
public:
	// Constructors and factories
	Knight(cocos2d::CCLayer* layer);
	~Knight();
	/**
	Creates a new knight.
	Generally, the knight spawn points will be marked on the tiled map
	and the position should be translated from the tile position.
	An allocated knight object should be passed in to be initialized.
	*/
	static Knight* initKnightInWorldAtPosition(Knight* knight, b2World* world, b2Vec2& position);


	// Getters and Setters



	// Virtual methods
	/**
	Updates the knight every tick
	*/
	virtual void update(float dt);
	
	/**
	 Attacks when it is close enough to death
	 **/
	virtual void attack();
	


	//
private:
	// Whether it's within the attack range 
	bool isNearDeath;

};

#endif
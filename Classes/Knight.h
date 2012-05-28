#pragma once
#ifndef KNIGHT_H
#define KNIGHT_H
#include "Enemy.h"

class Knight : public Enemy {
public:
	// Constructors and factories
	Knight();
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


	//
private:

};

#endif
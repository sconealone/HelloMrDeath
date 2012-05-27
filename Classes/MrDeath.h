
#ifndef MRDEATH_H
#define MRDEATH_H

#include "Character.h"
#include "Level.h"
#define MR_DEATH_SPEED 8.0f // TODO: placeholder
class Character;
class CCLayer;

class MrDeath : public Character {
public:
	MrDeath(cocos2d::CCLayer* layer);

	virtual ~MrDeath();

	// Jump function for Mr. Death
	void jump();

	virtual void moveLeft();

	virtual void moveRight();

	virtual void stopMoving();

	virtual void attack();
	
	virtual void specialAttack();

	bool getIsAttacking(){return isAttacking;}

	/**
	Updates Mr Death.
	Checks for collisions with enemies, and
	readjusts Mr Death's position based on
	his velocity.
	*/
	virtual void update();

private:
	/**
	Initializes all the animations that the sprite
	will need.
	They can be initialized when the level loads, and 
	then new actions can be created using them
	whenever needed from then on.
	*/
	virtual void initAnimations();

	/**
	Checks for collisions with enemies.
	If Mr. Death is currently attacking , there should be a seperate box for
	his scythe, and collisions should be checked there first.
	Uses the reference to the Level to find the enemies.
	*/
	void checkCollisions();

	/**
	Checks if Mr. Death has landed. Used to prevent
	double jumping.
	*/
	void checkIfLanded();

	void attackStop(cocos2d::CCNode* sender);
	void attackStart(cocos2d::CCNode* sender);
	
	

	// Instance Fields

	// fields denoting Mr. Death's state
	bool isAttacking;
	bool isMoving;
	bool isJumping;
	
	int soulGauge;
	
	cocos2d::CCLayer* layer;
	cocos2d::CCAnimation* attackStartupAnimation;
	cocos2d::CCAnimation* jumpAnimation;
	cocos2d::CCParticleSystem* specialAttackParticles;

	float previousYVelocity;
	bool wasAccelerating;

};
#endif
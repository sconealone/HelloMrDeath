
#ifndef MRDEATH_H
#define MRDEATH_H

#include "Character.h"
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
	virtual void initAnimations();


	void checkCollisions();
	void checkIfLanded();
	
	bool isAttacking;
	bool isMoving;
	bool isJumping;
	
	int soulGauge;
	
	void attackStop(cocos2d::CCNode* sender);
	void attackStart(cocos2d::CCNode* sender);


	cocos2d::CCLayer* layer;
	cocos2d::CCAnimation* attackStartupAnimation;
	cocos2d::CCAnimation* jumpAnimation;
	cocos2d::CCParticleSystem* specialAttackParticles;

	float previousYVelocity;
	bool wasAccelerating;

};
#endif
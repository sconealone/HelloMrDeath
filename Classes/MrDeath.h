
#ifndef MRDEATH_H
#define MRDEATH_H

#include "Character.h"
#include "Level.h"
#define MR_DEATH_SPEED 7.0f // TODO: placeholder
class Character;
class CCLayer;

class MrDeath : public Character {
public:
	MrDeath(cocos2d::CCLayer* layer);

	virtual ~MrDeath();

	// Jump function for Mr. Death
	void jump();


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
	
	// Override setPosition because Mr. Death needs
	// some tweaking since his sprite is not centred
	// in the middle of the bounding box.
	// The overridden setPosition adds an offset to
	// his sprite
	virtual void setPosition(b2Vec2& pos);
	virtual void setPosition(cocos2d::CCPoint pos);
	virtual void initFixtureDef(b2FixtureDef *fixDef, b2PolygonShape *shape);

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

	/**
	helper
	*/
	void setPosition();
	
	

	// Instance Fields

	// fields denoting Mr. Death's state
	bool isAttacking;
	bool isMoving;
	bool isJumping;
	
	int soulGauge;
	
	cocos2d::CCLayer* layer;
	/**
	The reason that  the attack is broken up into two set of animations
	is because the first frame of the attack animation, the scythe
	is behind Mr. Death and should not register as a hit with enemies.
	*/
	cocos2d::CCAnimation* attackStartupAnimation;
	cocos2d::CCAnimation* jumpAnimation;
	cocos2d::CCParticleSystem* specialAttackParticles;

	float previousYVelocity;
	bool wasAccelerating;
	float jumpSpeed;
	static const int SPRITE_BODY_OFFSET = 8;

};
#endif
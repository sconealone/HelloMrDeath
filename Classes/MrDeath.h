
#ifndef MRDEATH_H
#define MRDEATH_H

#include "Character.h"

class Character;
class CCLayer;

class MrDeath : public Character {
public:
	MrDeath(cocos2d::CCLayer* level);

	virtual ~MrDeath();

	// Jump function for Mr. Death
	void jump();

	virtual void moveLeft();

	virtual void moveRight();

	virtual void stopMoving();

	virtual void attack();

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

	bool isAttacking;
	bool isMoving;
	bool isJumping;

	void attackStop();

	cocos2d::CCLayer* level;
};
#endif
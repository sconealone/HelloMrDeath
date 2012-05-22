
#ifndef MRDEATH_H
#define MRDEATH_H

#include "Character.h"

class Character;
class Level;

class MrDeath : public Character {
public:

	// Jump function for Mr. Death
	void jump();

	virtual void moveLeft();

	virtual void moveRight();

	virtual void attack();

	virtual Character* initCharacterWithNameInWorld(Character* myChar, string name, b2World* world);

private:
	virtual void initActions2();
};
#endif
#ifdef TESTING
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


private:
	
};
#endif
#endif
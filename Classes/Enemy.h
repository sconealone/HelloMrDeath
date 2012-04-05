#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"

class Enemy : public Character {
public:

	// Update to see if collision with Mr. Death
	void update();

};

#endif
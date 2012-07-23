#ifndef CONLIST_H
#define CONLIST_H
#include "Box2D.h"
#include "cocos2d.h"
#include "Level.h"
#include "Knight.h"
#include "MrDeath.h"

class Level;
class Knight;
class MrDeath;

class ContactListener : public b2ContactListener{
	
public:
	/** Constructor **/
	ContactListener();
	~ContactListener();
	
	virtual void BeginContact(b2Contact* contact, Level* level);
	virtual void EndContact(b2Contact* contact, Level* level);
};


#endif

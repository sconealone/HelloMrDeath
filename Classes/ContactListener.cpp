/*
 *  ContactListener.cpp
 *  HelloMrDeath
 *
 *  Created by Olivia Zhang on 12-07-23.
 *  Copyright 2012 UBC. All rights reserved.
 *
 */

#include "ContactListener.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
using namespace std;

ContactListener::ContactListener() : b2ContactListener(){
}

ContactListener::~ContactListener(){
}

void ContactListener::BeginContact(b2Contact* contact, Level* level){

	
}

void ContactListener::EndContact(b2Contact* contact, Level* level){
	b2Fixture* fixA = contact->GetFixtureA();
	b2Fixture* fixB = contact->GetFixtureB();
	b2Body* bodyA = fixA->GetBody();
	b2Body* bodyB = fixB->GetBody();
	
	Knight* knight = level->getKnight();
	MrDeath* death = level->getDeath();
	
	if ((bodyA == knight->getBody() && bodyB == death->getBody()) ||
		bodyA == death->getBody() && bodyB == knight->getBody()) {
		int originHP = death->getHPValue();
		death->setHPValue(originHP-1);
		int newHP = death->getHPValue();
		cout << newHP;
	}
}
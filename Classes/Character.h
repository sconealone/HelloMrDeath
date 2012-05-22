#ifndef CHARACTER_H
#define CHARACTER_H

#include "cocos2d.h"
#include <CCMutableDictionary.h>
#include "Box2D.h"
#include "SimpleAudioEngine.h"
#include "MDUtil.h"
#include <string>

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include <windows.h>
#endif

/**
 * The Character is a sprite with a physics body
 * that it uses to get its position and interat
 * with the game world.
 */
class Character 
{
public:
	Character();

	virtual ~Character();
	/**
	Initializes a new character.
	By default calls the initSprite and initBody methods
	@param name should be the same name used to name all
	@param level a reference to the level the character will exist in
	the character's sprites and plist and sprite sheets.
	eg name.plist, name.png, name1.png
	*/
	virtual Character* initCharacterWithNameInWorld(Character* myChar,string name, b2World* world);

	b2World* getWorld(){return world;}

	// Attack value for each character
	int getAttackValue(){return attackValue;}

	// HP value for each character
	int getHPValue(){return hpValue;}

	cocos2d::CCSpriteBatchNode* getBatchNode(){return batchNode;}
	

	/**
	The position of the character in the world, in metres
	*/
	cocos2d::CCPoint getPosition(){return position;}

	void setPosition(cocos2d::CCPoint pos);

	// Move left function for characters
	virtual void moveLeft();

	// Move right function for characters
	virtual void moveRight();

	// Attack function for characters
	virtual void attack();
	

	/**
	The sprite that is the visual representation of the character
	*/
	CC_SYNTHESIZE(cocos2d::CCSprite*, sprite, Sprite);

	/**
	The physics body that is the physics representation
	of the the character
	*/
	CC_SYNTHESIZE(b2Body*, body, Body);

protected:
	int attackValue;
	int hpValue;

	

	 /**
	 * Initializes the animation for a sprite action.  
	 * Pure virtual function.
	 * @pre sprite is already initialized
	 * @param name must be the name of
	 * the files.
	 * eg. name.plist, name.png, name1.png
	 * @param numFrames will be the number of frames.
	 * There must be a file for each frame, numbered 
	 * starting from 1
	 * @param loop is if you want the animation to loop or not.
	 */
	 static cocos2d::CCAction* initAction(string name, int numFrames, bool loop);
	 
	cocos2d::CCAction* moveLeftAction;

	cocos2d::CCAction* moveRightAction;

	cocos2d::CCAction* attackAction;

private:
	b2World* world;
	cocos2d::CCPoint position;
	
	cocos2d::CCSpriteBatchNode* batchNode;

	/**
	Initializes a new sprite with the default frame 'name1.png'
	from a batchnode/sprite sheet 
	along with caching all the sprites so they can be used in animation
	actions.
	and the plist 'name.plist'
	@Pre the default frame of the sprite should be the first to appear 
	in the plist and the sprite sheet.
	*/
	virtual void initSprite(string name);

	/**
	Funtion for initializing the physics body.
	Takes the width and height of the sprite
	and uses it as the boundaries.
	Should be overriden by deriving class so that the
	actual boundaries more closesly match the drawing outline
	and not just the rectangular borders of the sprite.
	 */
	virtual void initBody();

	/**
	Initializes all the actions you may need.
	You should initialize attack, moveLeft, and moveRight.
	By default they do nothing.
	*/
	virtual void initActions();
};

#endif
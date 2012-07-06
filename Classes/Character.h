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



class MDSprite {
public:
	/**
	The sprite that is the visual representation of the character
	*/
	CC_SYNTHESIZE(cocos2d::CCSprite*, sprite, Sprite);
	virtual void setPosition(b2Vec2& pos);
	virtual void setPosition(cocos2d::CCPoint pos);

	/**
	The position of the character in the world, in metres
	*/
	b2Vec2 getPosition(){return position;} // deprecated
	b2Vec2 getPositionInMetres(){return position;}
	cocos2d::CCPoint getPositionInPixels(){return MDUtil::toCCPoint(position);}

	/**
	The bounding box is a rectangle that shows
	where the borders of the sprite are.  This
	means that it will generally be smaller than the size of
	the CCSprite, because there are often transparent bits
	of the CCSprite
	*/
	CC_SYNTHESIZE(cocos2d::CCRect, boundingBox, BoundingBox);

protected:
	b2Vec2 position;
};



/**
 * The Character is a sprite with a physics body
 * that it uses to get its position and interat
 * with the game world.
 */
class Character : public MDSprite
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
	bool getIsFacingRight(){return isFacingRight;}

	// Attack value for each character
	int getAttackValue(){return attackValue;}

	// HP value for each character
	CC_SYNTHESIZE(int, hpValue, HpValue);
	//int getHPValue(){return hpValue;}

	cocos2d::CCSpriteBatchNode* getBatchNode(){return batchNode;}
	


	virtual void setPosition(b2Vec2& pos);
	virtual void setPosition(cocos2d::CCPoint pos);

	// Move left function for characters
	virtual void moveLeft();

	// Move right function for characters
	virtual void moveRight();

	virtual void stopMoving();

	// Attack function for characters
	virtual void attack();
	
	/**
	Tells the character what to do on the next tick.
	For example, update could tell the character to 
	check it's physics body, and update its position accordingly,
	or it could use its AI to decide where  to move.
	*/
	virtual void update() = 0;


	/**
	The physics body that is the physics representation
	of the the character
	*/
	CC_SYNTHESIZE(b2Body*, body, Body);


protected:
	int attackValue;
	bool isFacingRight;
	b2World* world;
	float speed;
	float height; // metres
	float width; // metres
	

	 /**
	 * Initializes the action for a sprite animation.  
	 * The action will be retained.  You must release the memory
	 * when you are done.

	 The action is separate from the the animation because the idea
	 is to create the animation only once and save it, but to create
	 a new action every time you want it to be run.

	 * @pre sprite is already initialized
	 * @param the animation to run
	 * @param numFrames will be the number of frames.
	 * There must be a file for each frame, numbered 
	 * starting from 1
	 * @param loop is if you want the animation to loop or not.
	 */
	 static cocos2d::CCFiniteTimeAction* initAction(cocos2d::CCAnimation* animation, bool loop);
	 
	 /**
	 Initializes an animation
	 The animation will be retained. You must releae the memory.

	 The animation is seperate from the action because the idea is to 
	 initialize all the animations at the beginning of the level, and 
	 then to create a new action every time you want to run it.

	 @param name the name of the files. i.e. name.plist, name1.png, name.png. 
	 Individual frames must be numbered starting from 1.
	 @param numFrames the number of frames that the animation will last for
	 */
	 static cocos2d::CCAnimation* initAnimation(string name, int numFrames);
	 static cocos2d::CCAnimation* initAnimation(string name, int numFrames, int startFrame);
	 
	/**
	Funtion for initializing the physics body.
	Takes the width and height of the sprite
	and uses it as the boundaries.
	Should be overriden by deriving class so that the
	actual boundaries more closesly match the drawing outline
	and not just the rectangular borders of the sprite.
	 */
	virtual void initBody();
	 // Standard animations that all characters will need
	cocos2d::CCAnimation* moveLeftAnimation;
	cocos2d::CCAnimation* moveRightAnimation;
	cocos2d::CCAnimation* standStillAnimation;
	cocos2d::CCAnimation* attackAnimation;

	cocos2d::CCFiniteTimeAction* attackAction;

private:
	
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
	Initializes all the actions you may need.
	You should initialize stand still (default),
	attack, moveLeft, and moveRight.
	By default they do nothing.
	*/
	virtual void initAnimations() = 0;
	void setPosition();
};

#endif
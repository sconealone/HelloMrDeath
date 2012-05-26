#include "Character.h"
using namespace cocos2d;

Character::Character() {
	attackValue = 1;
	hpValue = 0;
	sprite = NULL;
	body = NULL;
	batchNode = NULL;
	moveLeftAnimation = NULL;
	moveRightAnimation = NULL;
	attackAnimation = NULL;
	standStillAnimation = NULL;
}

Character::~Character() {
}

/**
Appends two strings together without 
mutating either of the arguments.
@param front the string that will be at the front
@param back the string that will be at the back
*/
string myAppend(string front, string back) {
	string append = "";
	append += front;
	append += back;
	return append;
}

/**
Takes an integer and turns it into a
string
*/
string itostr(int num) {
	string str = "";
	int i = num;
	if (i < 0) {
		i = -i;
		myAppend("-", str);
	}
	char digit[2];
	digit[1] = '\0';
	do {
		digit[0] = i % 10 + ((int) '0');
		str = myAppend(string(digit), str);
	} while (i = i / 10);
	return str;
}

void Character::initSprite(string name) {
	string plist = myAppend(name, ".plist");
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(plist.c_str());

	string batchNodeStr = myAppend(name, ".png");
	this->batchNode = cocos2d::CCSpriteBatchNode::batchNodeWithFile(batchNodeStr.c_str());

	string spriteName = myAppend(name, "1");
	spriteName += ".png";
	sprite = CCSprite::spriteWithSpriteFrameName(spriteName.c_str());
}

CCAnimation* Character::initAnimation(string spriteName, int numFrames) {
	return initAnimation(spriteName, numFrames, 1);
}

CCAnimation* Character::initAnimation(string spriteName, int numFrames, int startFrame) {
	
	CCMutableArray<CCSpriteFrame *> *animationFrames = new CCMutableArray<cocos2d::CCSpriteFrame *>;
	for (int i = startFrame; i < numFrames + startFrame; ++i) {
		string frameName = myAppend(spriteName, itostr(i));
		frameName += ".png";
		animationFrames->addObject((CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName.c_str())));
	}
	
	const float FRAMETIME = 5 * 1.0f / 60.0f;
	CCAnimation *animation = CCAnimation::animationWithFrames(animationFrames, FRAMETIME);
	delete animationFrames;
	animation->retain();
	return animation;
}



CCFiniteTimeAction* Character::initAction(CCAnimation* animation, bool loop) {
	CCActionInterval *action = CCAnimate::actionWithAnimation(animation, false);
	//ccTime duration = animation->getDelay() * numFrames;
	//action->setDuration(duration);
	if (loop) {
		action = CCRepeatForever::actionWithAction(action);
	}
	action->retain();
	return action;
}

void Character::initBody() {
	CCSize size = sprite->getContentSize();
	float height = MDUtil::pixelsToMetres(size.height);
	float width = MDUtil::pixelsToMetres(size.width);

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f,0.0f);
	bodyDef.fixedRotation = true;
	bodyDef.userData = this;
	body = world->CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox(width,height);

	b2FixtureDef fixDef;
	fixDef.shape = &shape;
	fixDef.density = 0.8f; // TODO: placeholder value. please replace
	fixDef.friction = 0.0f;

	body->CreateFixture(&fixDef);
}

Character* Character::initCharacterWithNameInWorld(Character* myChar, string name, b2World* world) {
	myChar->world = world;
	myChar->initSprite(name);
	myChar->initBody();
	myChar->initAnimations();
	return myChar;
}

void Character::attack() {
}

void Character::moveLeft() {
}

void Character::moveRight() {
}

void Character::stopMoving() {
}

void Character::setPosition(CCPoint pos) {
	position = pos;
	sprite->setPosition(position);
	body->SetTransform(b2Vec2(MDUtil::pixelsToMetres(position.x),MDUtil::pixelsToMetres(position.y)),body->GetAngle());
}
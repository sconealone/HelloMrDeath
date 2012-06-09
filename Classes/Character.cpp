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
	isFacingRight = true;
	speed = 4.0f; // default speed value
	height = -1;
	width = -1;
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
	if (height <= 0 || width <= 0) {
		CCSize size = sprite->getContentSize();
		height = MDUtil::pixelsToMetres(size.height);
		width = MDUtil::pixelsToMetres(size.width);
	}
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f,0.0f);
	bodyDef.fixedRotation = true;
	bodyDef.userData = this;
	body = world->CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox(width/2,height/2);

	b2FixtureDef fixDef;
	fixDef.shape = &shape;
	fixDef.density = 1.0f; // TODO: placeholder value. please replace
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
	CCFiniteTimeAction* flip = NULL;
	if (isFacingRight) { 
		flip = CCFlipX::actionWithFlipX(true);
		sprite->runAction(flip);
	}
	isFacingRight = false;
	body->SetLinearVelocity(b2Vec2(-8.0f,body->GetLinearVelocity().y));
}



void Character::moveRight() {
	CCFiniteTimeAction* flip = NULL;
	if (!isFacingRight) {
		flip = CCFlipX::actionWithFlipX(false);
		sprite->runAction(flip);
	}
	isFacingRight = true;
	body->SetLinearVelocity(b2Vec2(8.0f,body->GetLinearVelocity().y));
}

void Character::stopMoving() {
	body->SetLinearVelocity(b2Vec2(0.0f, body->GetLinearVelocity().y));
}

void Character::setPosition(b2Vec2 &pos) {
	position = pos;
	sprite->setPosition(ccp(MDUtil::metresToPixels(pos.x), MDUtil::metresToPixels(pos.y)));
	body->SetTransform(position,body->GetAngle());
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
void Character::setPosition(CCPoint &pos) {
	position = b2Vec2(MDUtil::pixelsToMetres(pos.x),MDUtil::pixelsToMetres(pos.y));
	sprite->setPosition(pos);
	body->SetTransform(position, body->GetAngle());
}
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
void Character::setPosition(CCPoint pos) {
	position = b2Vec2(MDUtil::pixelsToMetres(pos.x),MDUtil::pixelsToMetres(pos.y));
	sprite->setPosition(pos);
	body->SetTransform(position, body->GetAngle());
}
#endif 

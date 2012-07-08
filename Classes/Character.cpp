#include "Character.h"
using namespace cocos2d;

Character::Character() {
	attackValue = 1;
	hpValue = 1;
	sprite = NULL;
	body = NULL;
	batchNode = NULL;
	moveLeftAnimation = NULL;
	moveRightAnimation = NULL;
	attackAnimation = NULL;
	standStillAnimation = NULL;
	isFacingRight = true;
	speed = 4.0f; // default speed value
	height = 0;
	width = 0;
	boundingBox = CCRect(0,0,width,height);
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
		boundingBox.size = CCSize(width,height);
	}
	b2BodyDef bodyDef;
	initBodyDef(&bodyDef);
	body = world->CreateBody(&bodyDef);

	
	b2FixtureDef fixDef;
	b2PolygonShape shape;
	shape.SetAsBox(width/2,height/2);
	initFixtureDef(&fixDef, &shape);

	body->CreateFixture(&fixDef);
}

void Character::initBodyDef(b2BodyDef *bodyDef) {
	bodyDef->type = b2_dynamicBody;
	bodyDef->position.Set(0.0f,0.0f);
	bodyDef->fixedRotation = true;
	bodyDef->userData = this;
}

void Character::initFixtureDef(b2FixtureDef *fixDef, b2PolygonShape *shape) {

	fixDef->shape = shape;
	fixDef->density = 1.0f; // TODO: placeholder value. please replace
	fixDef->friction = 3.0f;
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


void Character::moveLeft(float vel) {
	CCFiniteTimeAction* flip = NULL;
	if (isFacingRight) { 
		flip = CCFlipX::actionWithFlipX(true);
		sprite->runAction(flip);
	}
	isFacingRight = false;
	body->SetLinearVelocity(b2Vec2(vel*-1,body->GetLinearVelocity().y));
}



void Character::moveRight(float vel) {
	CCFiniteTimeAction* flip = NULL;
	if (!isFacingRight) {
		flip = CCFlipX::actionWithFlipX(false);
		sprite->runAction(flip);
	}
	isFacingRight = true;
	body->SetLinearVelocity(b2Vec2(vel,body->GetLinearVelocity().y));
}

void Character::stopMoving() {
	body->SetLinearVelocity(b2Vec2(0.0f, body->GetLinearVelocity().y));
}

void Character::setPosition(b2Vec2 &pos) {
	MDSprite::setPosition(pos);
	setPosition();
}


void Character::setPosition(CCPoint pos) {
	MDSprite::setPosition(pos);
	setPosition();
}

void Character::setPosition() {
	body->SetTransform(position, body->GetAngle());
}

void Character::update() {	
	b2Vec2 vec = body->GetPosition();
	setPosition(vec);
}

void MDSprite::setPosition(b2Vec2 &pos) {
	position = pos;
	sprite->setPosition(ccp(MDUtil::metresToPixels(pos.x), MDUtil::metresToPixels(pos.y)));
}


void MDSprite::setPosition(CCPoint pos) {
	position = b2Vec2(MDUtil::pixelsToMetres(pos.x),MDUtil::pixelsToMetres(pos.y));
	sprite->setPosition(pos);
}

MDSprite::MDSprite() {
	sprite = NULL;
	position = b2Vec2(0.0f, 0.0f);
	boundingBox = CCRect();
}

void MDSprite::initMDSprite(MDSprite* mdSprite, cocos2d::CCSprite* sprite,
							cocos2d::CCPoint position, cocos2d::CCRect boundingBox) {
	mdSprite->sprite = sprite;
	CCSize size = sprite->getContentSize();
	mdSprite->setPosition(position);
	mdSprite->boundingBox = CCRect(boundingBox);
	CCPoint oldPos = mdSprite->boundingBox.origin;
	mdSprite->boundingBox.origin = ccp(oldPos.x - size.width/2 + position.x,
									   oldPos.y - size.height/2 + position.y);
}
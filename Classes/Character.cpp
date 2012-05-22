#include "Character.h"
using namespace cocos2d;

Character::Character() {
	attackValue = 1;
	hpValue = 0;
	sprite = NULL;
	body = NULL;
	referenceCount = 0;
	batchNode = NULL;
	moveLeftAction = NULL;
	moveRightAction = NULL;
	attackAction = NULL;
}

Character::~Character() {
	CC_SAFE_RELEASE_NULL(sprite);
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

CCAction* Character::initAction(string spriteName, int numFrames, bool loop) {
	CCMutableArray<CCSpriteFrame *> *animationFrames = new CCMutableArray<cocos2d::CCSpriteFrame *>;
	for (int i = 0; i < numFrames; ++i) {
		string frameName = myAppend(spriteName, itostr(i + 1));
		frameName += ".png";
		animationFrames->addObject((CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(frameName.c_str())));
	}
	
	CCAnimation *animation = CCAnimation::animationWithFrames(animationFrames);
	CCAction *action = CCAnimate::actionWithAnimation(animation, true);
	
	delete animationFrames;

	return action;
}

void Character::initBody() {
	CCSize size = sprite->getContentSize();
	float height = Level::pixelsToMetres(size.height);
	float width = Level::pixelsToMetres(size.width);

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f,0.0f);
	bodyDef.fixedRotation = true;
	bodyDef.userData = this;
	body = level->getWorld()->CreateBody(&bodyDef);

	b2PolygonShape shape;
	shape.SetAsBox(width,height);

	b2FixtureDef fixDef;
	fixDef.shape = &shape;
	fixDef.density = 0.8f; // TODO: placeholder value. please replace
	fixDef.friction = 0.0f;

	body->CreateFixture(&fixDef);
}

Character* Character::createCharacterWithNameInLevel(string name, Level* level) {
	Character* myChar = new Character;
	myChar->level = level;
	myChar->initSprite(name);
	myChar->initBody();
	++(myChar->referenceCount);
	return myChar;
}

void Character::attack() {
	sprite->runAction(attackAction);
}

void Character::moveLeft() {
	sprite->runAction(moveLeftAction);
}

void Character::moveRight() {
	sprite->runAction(moveRightAction);
}

void Character::retain() {
	++referenceCount;
}

void Character::release() {
	--referenceCount;
	if (referenceCount <= 0) {
		delete this;
	}
}

void Character::initActions() {
}

void Character::setPosition(CCPoint pos) {
	position = pos;
	sprite->setPosition(position);
	body->SetTransform(b2Vec2(Level::pixelsToMetres(position.x),Level::pixelsToMetres(position.y)),body->GetAngle());
}
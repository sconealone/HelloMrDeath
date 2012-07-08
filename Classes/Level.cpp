#include <stdlib.h>
#include <iostream>
#include "Level.h"

using namespace std;
using namespace cocos2d;

Level::Level() {
	gameLayer = NULL;
	controlLayer = NULL;
	enemies = NULL;
	world = NULL;
	death = NULL;
	isTouching = false;
	platformsLayer = NULL;
	tiledMap = NULL;
	collidableLayer = NULL;
	knight = NULL;
	checkpoints = NULL;
	numCheckpoints = 0;
	alreadyWon = false;
}


Level::~Level() {
	if (world != NULL) delete world;
	world = NULL;
	if (death != NULL) delete death;
	death = NULL;
	if (knight != NULL) delete knight;
	knight = NULL;
	if (checkpoints != NULL) delete [] checkpoints;
	checkpoints = NULL;
}

MrDeath* Level::getDeath(){
	return death;
}

Knight* Level::getKnight(){
	return knight;
}

bool Level::init() {
	bool initSuccessful = false;
	
	// flag to detect memory leaks
	#if	(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif
	
	do {
		CC_BREAK_IF(!super::init());
		
		
		gameLayer = CCLayer::node();
		controlLayer = CCLayer::node();
		this->addChild(gameLayer, 0);
		this->addChild(controlLayer,1);
		this->setIsTouchEnabled(true);

		initButtons();
		initWorld();
		initPC();
		initCheckpoints();
		
		this->schedule(schedule_selector(Level::update), TIMESTEP);
		initSuccessful = true;
	} while (0);
	CC_ASSERT(initSuccessful);
	
	return initSuccessful;
}

void Level::initWorld() {
	const float GRAVITY = -70.0f;
	b2Vec2 gravity = b2Vec2(0.0f, GRAVITY);
	bool doSleep = true;
	world = new b2World(gravity);
	world->SetAllowSleeping(doSleep);

	initBg();
	initPlatformsFromTiledMap();
	initWorldBorders();
}


void Level::initWorldBorders() {
	CCSize mapSize = tiledMap->getContentSize();
	mapSize.height = MDUtil::pixelsToMetres(mapSize.height);
	mapSize.width = MDUtil::pixelsToMetres(mapSize.width);
	const float BOTTOM_HEIGHT = -5.0f;

	b2BodyDef borderBodyDef;
	borderBodyDef.position.Set(0.0f, 0.0f);
	b2Body *borderBody = world->CreateBody(&borderBodyDef);
	
	b2EdgeShape floorShape;
	floorShape.Set(b2Vec2(0.0f, BOTTOM_HEIGHT), b2Vec2(mapSize.width, BOTTOM_HEIGHT));
	borderBody->CreateFixture(&floorShape, 0.0f);

	b2EdgeShape leftWallShape;
	leftWallShape.Set(b2Vec2(0.0f, BOTTOM_HEIGHT), b2Vec2(0.0f, mapSize.height + 1));
	borderBody->CreateFixture(&leftWallShape, 0.0f);

	b2EdgeShape rightWallShape;
	rightWallShape.Set(b2Vec2(mapSize.width, BOTTOM_HEIGHT), b2Vec2(mapSize.width, mapSize.height + 1));
	borderBody->CreateFixture(&rightWallShape, 0.0f);
}



void Level::initBg() {
	initWeather();
	tiledMap = CCTMXTiledMap::tiledMapWithTMXFile("test_map.tmx");
	platformsLayer = tiledMap->layerNamed("Platforms");
	collidableLayer = tiledMap->layerNamed("Collidable");
	collidableLayer->setIsVisible(false);
	gameLayer->addChild(tiledMap, -1);
}

void Level::initWeather() {
	CCParticleSystem* emitter;
	emitter = CCParticleRain::node();
	emitter->setTexture(CCTextureCache::sharedTextureCache()->addImage("attack_released.png"));
	emitter->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width/2, CCDirector::sharedDirector()->getWinSize().height));
	this->addChild(emitter);
}



void Level::initPC() {
	
		// TODO: test
		death = new MrDeath(gameLayer);
		death->initCharacterWithNameInWorld(death,"death",world);
		gameLayer->addChild(death->getBatchNode(), 0);
		CCTMXObjectGroup *objectGroup = tiledMap->objectGroupNamed("Objects");
		CCStringToStringDictionary *spawnDictionary = objectGroup->objectNamed("DeathSpawn");
		float xCoord = spawnDictionary->objectForKey("x")->toFloat();
		float yCoord = spawnDictionary->objectForKey("y")->toFloat();
		death->setPosition(ccp(xCoord, yCoord));
		death->getBatchNode()->addChild(death->getSprite(), 1);
		
	
		knight = new Knight(this);
		knight-> initCharacterWithNameInWorld(knight, "knight", world);
		gameLayer->addChild(knight->getBatchNode(), 0);
		knight->setPosition(ccp(250,75));
		knight->getBatchNode()->addChild(knight->getSprite(), 1);
		

		centreCamera();

}


void Level::initCheckpoints() {
	levelEnd = MDSprite();

	CCSprite* sprite = CCSprite::spriteWithFile("lamp-t.png");
	CCTMXObjectGroup *obj = tiledMap->objectGroupNamed("Objects");
	CCStringToStringDictionary *dict = obj->objectNamed("LevelEnd");
	float x = dict->objectForKey("x")->toFloat();
	float y = dict->objectForKey("y")->toFloat();
	CCPoint position = ccp(x, y);

	CCRect boundingBox(26.0f, 45.0f, 12.0f, 25.0f);

	MDSprite::initMDSprite(&levelEnd, sprite, position, boundingBox);

	gameLayer->addChild(sprite, 1);
}

CCScene* Level::scene() {
	CCScene* scene = NULL;
	do {
		scene = CCScene::node();

		CC_BREAK_IF(!scene);
		
		Level* layer = Level::node();
	
		
		CC_BREAK_IF(!layer);
		scene->addChild(layer);
	} while(0);
	return scene;

}

void Level::update(float dt) {
	checkInput();
	const int velocityIterations = 8;
	const int positionIterations = 3;
	world->Step(TICK_TIME, velocityIterations, positionIterations);
	death->update();
	knight->update();
	checkPitfalls();
	checkDeaths();
	checkCheckpoints();
	centreCamera();
}

bool Level::isRightArrow(float x, float y){
	return (x >= rightbutton->getPosition().x-25 &&
		x <= rightbutton->getPosition().x+25 &&
		y >= rightbutton->getPosition().y-15 &&
		y <= rightbutton->getPosition().y+15);
}

bool Level::isLeftArrow(float x, float y) {
	return (x >= leftbutton->getPosition().x-25 &&
		x <= leftbutton->getPosition().x+25 &&
		y >= leftbutton->getPosition().y-15 &&
		y <= leftbutton->getPosition().y+15);
}

bool Level::isRightArrow(CCPoint &point) {
	return CCRect::CCRectContainsPoint(rightbutton->boundingBox(), point);
}

bool Level::isLeftArrow(CCPoint &point) {
	return CCRect::CCRectContainsPoint(leftbutton->boundingBox(), point);
}

void Level::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent){
	CCTouch *touch = (CCTouch *)pTouches->anyObject();
	CCPoint location = touch->locationInView(touch->view());
	location = CCDirector::sharedDirector()->convertToGL(location);
	
	isTouching = true;
	
	if (isRightArrow(location) && isLeftArrow(location)) {
		// do nothing. don't press arrows at the same time.
	} else if (isRightArrow(location)) {
		death->moveRight(8.0f);
	} else if (isLeftArrow(location)) {
		death->moveLeft(8.0f);
	}
	
}

void Level::ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent) {
	isTouching = false;
	death->stopMoving();
}



void Level::initPlatformsFromTiledMap() {
	CCSize mapSize = tiledMap->getMapSize();
	for (int i = 0; i < mapSize.height; ++i) {
		for (int j = 0; j < mapSize.width; ++j) {
			if (collidableLayer->tileGIDAt(ccp((float)j, (float)i))) {
				int lastColumn = j;
				
				while (lastColumn < mapSize.width && 
					   collidableLayer->tileGIDAt(ccp((float)lastColumn, (float)i))) {
					++lastColumn;
				}
				float width = MDUtil::tilesToMetres((float)lastColumn - j);
				float height = MDUtil::tilesToMetres(1.0f);
				float centerX = MDUtil::tilesToMetres((float)j) + width/2;
				float centerY = MDUtil::tilesToMetres(mapSize.height - ((float) i + 0.5f));
				createPlatformBody(width, height, centerX, centerY);
				j = lastColumn;
			}
		}
	}
}

void Level::createPlatformBody(float width, float height, float centerX, float centerY) {
	b2BodyDef def;
	b2Body* body = world->CreateBody(&def);
	b2PolygonShape platform;
	b2Vec2 center(centerX, centerY);
	platform.SetAsBox(width/2, height/2, center, 0.0f);
	body->CreateFixture(&platform, 0.0f);
}

void Level::initButtons() {
	// left button implementation
	leftbutton = CCSprite::spriteWithFile("left_pressed.png");
	leftbutton->setPosition(ccp(50,40));
	controlLayer->addChild(leftbutton);

	// right button implementation
	rightbutton = CCSprite::spriteWithFile("right_pressed.png");
	rightbutton->setPosition(ccp(100,40));
	controlLayer->addChild(rightbutton);
	
	
	CCPoint specialPos = ccp(400,40);
	CCPoint jumpPos = ccp(450,40);
	CCPoint  attackPos = ccp(450,90);


	// attack button implementation
	attackbutton = CCMenuItemImage::itemFromNormalImage("attack_released.png",
														"attack_pressed.png",
														this,
														menu_selector(Level::attack));
	attackbutton->setPosition(attackPos);
	CCMenu *aMenu = CCMenu::menuWithItems(attackbutton, NULL);
	aMenu->setPosition(CCPointZero);
	controlLayer->addChild(aMenu, 1);
	
	// jump button implementation
	jumpbutton = CCMenuItemImage::itemFromNormalImage("jump_released.png",
													  "jump_pressed.png",
													  this,
													  menu_selector(Level::jump));
	jumpbutton->setPosition(jumpPos);
	CCMenu *jMenu = CCMenu::menuWithItems(jumpbutton, NULL);
	jMenu->setPosition(CCPointZero);
	controlLayer->addChild(jMenu, 1);
	
	// special attack button
	specialbutton = CCMenuItemImage:: itemFromNormalImage("special_released.png",
														  "special_pressed.png",
														  this,
														  menu_selector(Level::specialAttack));
	specialbutton->setPosition(specialPos);
	CCMenu *sMenu = CCMenu::menuWithItems(specialbutton, NULL);
	sMenu->setPosition(CCPointZero);
	controlLayer->addChild(sMenu, 1);
	
													

}

void Level::attack(CCObject* sender){
	death->attack();
}

void Level::specialAttack(CCObject* sender) {
	death->specialAttack();
}

void Level::jump(CCObject* sender) {
	death->jump();
}

void Level::checkInput() {
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	checkKeyboard();
	#endif
}

void Level::centreCamera() {
	CCSize mapsize = tiledMap->getContentSize();
	CCSize winsize = CCDirector::sharedDirector()->getWinSize();
	CCPoint winpos = gameLayer->getPosition();
	CCPoint newpos = winpos;
	CCPoint deathpos = death->getSprite()->getPosition();
	float deathForwardOffset = winsize.width/2;
	float deathBackwardOffset = winsize.width - deathForwardOffset;
	float deathLowerOffset = 0.8f*winsize.height;
	float deathUpperOffset = winsize.height - deathLowerOffset;

	if (death->getIsFacingRight()) {
		if (!(deathpos.x <= deathForwardOffset || 
			  deathpos.x >= mapsize.width - deathBackwardOffset)) {
				newpos.x = -(deathpos.x - deathForwardOffset);
		}
	}
	else { // death is facing left
		if (!(deathpos.x <= deathBackwardOffset ||
			  deathpos.x >= mapsize.width - deathForwardOffset)) {
				  newpos.x = -(deathpos.x - deathBackwardOffset);
		}
	}

	if (!(deathpos.y <= deathLowerOffset ||
		  deathpos.y >= mapsize.height - deathUpperOffset)) {
			newpos.y = -(deathpos.y - deathLowerOffset);
	}

	gameLayer->setPosition(newpos);
}

void Level::checkPitfalls() {
	if (death->getPositionInMetres().y <= -3.0f) {
		death->setHpValue(0);
	}
}

void Level::checkDeaths() {
	// TODO: placeholder code
	if (!death->getHpValue()) {
		death->setHpValue(5);
		CCTMXObjectGroup *respawn = tiledMap->objectGroupNamed("Objects");
		CCStringToStringDictionary *respawnD = respawn->objectNamed("DeathSpawn");
		death->setPosition(ccp(respawnD->objectForKey("x")->toFloat(), respawnD->objectForKey("y")->toFloat()));
		CCLabelTTF *label = CCLabelTTF::labelWithString("You died.", "Artial", 32);
		gameLayer->setPosition(ccp(0.0f, 0.0f));
		label->setColor(ccc3(0xff, 0x0, 0x0));
		label->setPosition(ccpAdd(gameLayer->getPosition(), 
								  ccp(CCDirector::sharedDirector()->getWinSize().width/2, 
									  CCDirector::sharedDirector()->getWinSize().height/2)));
		gameLayer->addChild(label);
	}
}

// TODO implement behaviour for reaching the end of level, checkpoints
void Level::checkCheckpoints() {
	if (checkpoints != NULL && numCheckpoints > 0) {
		MDSprite* reachedCheckpoint = NULL;
		for (int i = 0; i < numCheckpoints; ++i) {
			bool mrDeathReachedCheckpoint = 
				CCRect::CCRectIntersectsRect(death->getBoundingBox(), 
				   							 checkpoints[i].getBoundingBox());
			if (mrDeathReachedCheckpoint) {
				reachedCheckpoint = &checkpoints[i];
				break;
			}
		}
		if (reachedCheckpoint != NULL) {
			// set respawn point to checkpoint
		}
	}

	bool mrDeathReachedLevelEnd =
		!alreadyWon && CCRect::CCRectIntersectsRect(death->getBoundingBox(),
				  								    levelEnd.getBoundingBox());
	if (mrDeathReachedLevelEnd) {
		// end the level
		alreadyWon = true;
		CCLabelTTF *label = CCLabelTTF::labelWithString("You have reached the end of this level.", "Artial", 32);
		label->setColor(ccc3(0x0, 0xff, 0x0));
		label->setPosition(ccpAdd(death->getPositionInPixels(), ccp(-150.0f, 50.0f)));
		gameLayer->addChild(label);
	}
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
void Level::checkKeyboard() {
	const short KEY_UP = (short) 0x8000;

	// VK codes
	const int W = 0x57;
	const int A = 0x41;
	const int D = 0x44;
	const int E = 0x45;
	const int R = 0x52;
	short wasJumpPressed = GetKeyState(W);
	short wasLeftPressed = GetKeyState(A);
	short wasRightPressed = GetKeyState(D);
	short wasAttackPressed = GetKeyState(E);
	short wasSpecialPressed = GetKeyState(R);
	// vertical movement is handled seperately from horizontal
	if (KEY_UP & wasJumpPressed) {
		death->jump();
	}
	if (KEY_UP & wasAttackPressed) {
		
		death->attack();	
	}
	if (KEY_UP & wasSpecialPressed) {
		death->specialAttack();
	}
	if (!isTouching) {
		if (KEY_UP & wasLeftPressed & wasRightPressed)	{
			// do nothing, don't press keys at the same time doofus
		}
		else if (KEY_UP & wasLeftPressed)	{
			death->moveLeft();
		}
		else if (KEY_UP & wasRightPressed)	{
			death->moveRight();
		}
		else	{
			death->stopMoving();
		}
	}

}
#endif


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
}

Level::~Level() {
	delete world;
	world = NULL;
	delete death;
	death = NULL;
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
		
		this->schedule(schedule_selector(Level::update), TIMESTEP);
		initSuccessful = true;
	} while (0);
	CC_ASSERT(initSuccessful);
	
	return initSuccessful;
}

void Level::initWorld() {
	const float GRAVITY = -80.0f;
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
	b2BodyDef floorBodyDef;
	floorBodyDef.position.Set(0.0f, 0.0f);
	b2Body *floorBody = world->CreateBody(&floorBodyDef);
	b2EdgeShape floorShape;
	floorShape.Set(b2Vec2(0.0f, 0.0f), b2Vec2(MDUtil::metresToPixels(mapSize.width), 0.0f));
	floorBody->CreateFixture(&floorShape, 0.0f);
}



void Level::initBg() {
	initWeather();
	tiledMap = CCTMXTiledMap::tiledMapWithTMXFile("test_map_single_tile.tmx");
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
		death = new MrDeath(this);
		death->initCharacterWithNameInWorld(death,"death",world);
		gameLayer->addChild(death->getBatchNode(), 0);
		CCTMXObjectGroup *objectGroup = tiledMap->objectGroupNamed("Objects");
		CCStringToStringDictionary *spawnDictionary = objectGroup->objectNamed("DeathSpawn");
		float xCoord = spawnDictionary->objectForKey("x")->toFloat();
		float yCoord = spawnDictionary->objectForKey("y")->toFloat();
		death->setPosition(ccp(xCoord, yCoord));
		death->getBatchNode()->addChild(death->getSprite(), 1);
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
		death->moveRight();
	} else if (isLeftArrow(location)) {
		death->moveLeft();
	}
	
}

void Level::ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent) {
	isTouching = false;
	death->stopMoving();
}


/**
 * Takes a y-coordinate from a system that has the origin fixed at the top-left
 * (like Tiled) and converts it to a y-coordinate from a system that has the 
 * origin at the bottom-left (like Box2D, cocos2d).
 * If the two systems are superimposed over each other, the points should 
 * be at the same spot.
 */
float coordsFromTopToCoordsFromBottom(float yCoord, float height) {
	return height - yCoord;
}

void Level::initPlatformsFromTiledMap() {

	b2Vec2 center(MDUtil::tilesToMetres(7.5f), MDUtil::tilesToMetres(tiledMap->getMapSize().height - 21.5f));
	b2BodyDef def;
	def.position.Set(center.x, center.y);
	b2Body* body = world->CreateBody(&def);
	b2PolygonShape shape;
	float boxRadius = 0.1f;
	shape.SetAsBox(boxRadius, boxRadius);
	body->CreateFixture(&shape, 0.0f);


	/*
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
				float centerY = MDUtil::tilesToMetres(coordsFromTopToCoordsFromBottom((float)i, mapSize.height) + 0.5f);
				float centerX = MDUtil::tilesToMetres((float)j) + height / 2.0f;
				createPlatformBody(width, height, centerX, centerY);
				j = lastColumn;
			}
		}
	}
	*/
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


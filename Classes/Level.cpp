#include "Level.h"
using namespace cocos2d;

Level::Level() {
	gameLayer = NULL;
	controlLayer = NULL;
	platformLayer = NULL;
	enemies = NULL;
	world = NULL;
	death = NULL;
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
		
		this->addChild(gameLayer, 0);

		initWorld();

		initPC();
		
		this->schedule(schedule_selector(Level::update), TIMESTEP);
		initSuccessful = true;
	} while (0);
	CC_ASSERT(initSuccessful);
	return initSuccessful;
}

void Level::initWorld() {
	const float GRAVITY = -100.0f;
	b2Vec2 gravity = b2Vec2(0.0f, GRAVITY);
	bool doSleep = true;
	world = new b2World(gravity);
	world->SetAllowSleeping(doSleep);
	initWorldBorders();
}


void Level::initWorldBorders()
{
	b2BodyDef borderBodyDef;
	borderBodyDef.type = b2_staticBody;
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	b2Body *body = world->CreateBody(&borderBodyDef);
	const int numCorners = 4;
	b2Vec2 verticies[numCorners];
	const float OFFSCREEN_OFFSET = 10.0f;
	verticies[0].Set(0.0f, -OFFSCREEN_OFFSET);
	verticies[1].Set(0.0f, MDUtil::pixelsToMetres(winSize.height) + OFFSCREEN_OFFSET);
	verticies[2].Set(MDUtil::pixelsToMetres(winSize.width), MDUtil::pixelsToMetres(winSize.height) + OFFSCREEN_OFFSET);
	verticies[3].Set(MDUtil::pixelsToMetres(winSize.width), -OFFSCREEN_OFFSET);
	
	b2ChainShape borderBox;
	borderBox.CreateChain(verticies, numCorners);
	body->CreateFixture(&borderBox, 0.0f);
	
}

void Level::initPC() {
	
		// TODO: test
		death = new MrDeath(this);
		death->initCharacterWithNameInWorld(death,"death",world);
		gameLayer->addChild(death->getBatchNode(), 0);
		death->setPosition(ccp(100.0f, 100.0f));
		
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

bool Level::isPlatform(CCPoint tileCoord) {
	return platformLayer->tileGIDAt(tileCoord) ? true : false;
}

void Level::initPlatformsFromTiledMap() {
	CCSize mapSize = tiledMap->getMapSize();
	for (int i = 0; i < mapSize.height; ++i) {
		for (int j = 0; j < mapSize.width; ++j) {
			if (isPlatform(ccp(i,j))) {
				int lastColumn = j;
				while (lastColumn < mapSize.width && isPlatform(ccp(i,lastColumn))) {
					++lastColumn;
				}
				float width = MDUtil::tilesToMetres((float)lastColumn - j);
				float height = MDUtil::tilesToMetres(1.0f);
				float centerY = MDUtil::tilesToMetres(coordsFromTopToCoordsFromBottom((float)i, mapSize.height) + 0.5f);
				float centerX = MDUtil::tilesToMetres((float)j) + height / 2.0f;
				createPlatformBody(width, height, (float)i, (float)j);
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
	platform.SetAsBox(width, height, center, 0.0f);
}

void Level::checkInput() {
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	checkKeyboard();
	#else // is iOS
	checkTouches();
	#endif
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
void Level::checkKeyboard() {
	const short KEY_UP = (short) 0x8000;

	// VK codes
	const int W = 0x57;
	const int A = 0x41;
	const int D = 0x44;
	const int R = 0x52;
	short wasJumpPressed = GetKeyState(W);
	short wasLeftPressed = GetKeyState(A);
	short wasRightPressed = GetKeyState(D);
	short wasAttackPressed = GetKeyState(R);
	// vertical movement is handled seperately from horizontal
	if (KEY_UP & wasJumpPressed) {
		death->jump();
	}
	if (KEY_UP & wasAttackPressed) {
		death->attack();
	}
	if (KEY_UP & wasLeftPressed & wasRightPressed)
	{
		// do nothing, don't press keys at the same time doofus
	}
	else if (KEY_UP & wasLeftPressed)
	{
		death->moveLeft();
	}
	else if (KEY_UP & wasRightPressed)
	{
		death->moveRight();
	}
	else
	{
		death->stopMoving();
	}

}
#else
void Level::checkTouches() {
}
#endif
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

Level::~Level() {}

bool Level::init() {
	bool initSuccessful = false;
	
	// flag to detect memory leaks
	#if	(CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	do {
		CC_BREAK_IF(!super::init());
		
		this->schedule(schedule_selector(Level::update), TIMESTEP);
		initSuccessful = true;
	} while (0);
	CC_ASSERT(initSuccessful);
	return initSuccessful;
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

}


/**
 * Converts a size in tiles to a size in metres
 */
float tilesToMetres(float tileNumber) {
	const int PIXELS_PER_TILE = 32;
	const int PIXELS_PER_METRE = 64; // placeholder
	return tileNumber * PIXELS_PER_TILE / PIXELS_PER_METRE;
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
	return platformLayer->tileGIDAt(tileCoord);
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
				int width = tilesToMetres(lastColumn - j);
				int height = tilesToMetres(1);
				float centerY = tilesToMetres(coordsFromTopToCoordsFromBottom(i, mapSize.height) + 0.5f);
				float centerX = tilesToMetres(j) + height / 2.0f;
				createPlatformBody(width, height, i, j);
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

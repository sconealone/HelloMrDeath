#include "Level.h"
using namespace cocos2d;

Level::Level() {
	gameLayer = NULL;
	controlLayer = NULL;
	enemies = NULL;
	world = NULL;
	death = NULL;
}

Level::~Level() {}

bool Level::init() {
	bool initSuccessful = false;
	
	// flag to detect memory leaks
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

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
				float centerX = tilesToMetres(i + 0.5f);
				float centerY = tilesToMetres(j) + height / 2.0f;
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

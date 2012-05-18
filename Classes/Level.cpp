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
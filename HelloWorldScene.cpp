#include "HelloWorldScene.h"
//#include "SimpleAudioEngine.h"
#include "GameOverScene.h"

USING_NS_CC;

CCSprite* player;
CCSprite* target;
CCSprite* background;
HelloWorld::~HelloWorld()
{
	if (_targets)
	{
		_targets->release();
		_targets = NULL;
	}
	
	if (_projectiles)
	{
		_projectiles->release();
		_projectiles = NULL;
	}
	
	// cpp don't need to call super dealloc
	// virtual destructor will do this
}


CCScene* HelloWorld::scene()
{
	// 'scene' is an autorelease object
	CCScene *scene = CCScene::node();
	
	// 'layer' is an autorelease object
	HelloWorld *layer = HelloWorld::node();
	
	// add layer as a child to scene
	scene->addChild(layer);
	
	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    if ( !CCColorLayer::initWithColor( ccc4(0,0,0,0) ) )	{
		return false;
	}
	
	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.
	
	// add a "close" icon to exit the progress. it's an autorelease object
	CCMenuItemImage *pCloseItem = CCMenuItemImage::itemFromNormalImage(
																	   "CloseNormal.png",
																	   "CloseSelected.png",
																	   this,
																	   menu_selector(HelloWorld::menuCloseCallback) );
	pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );
	
	// create menu, it's an autorelease object
	CCMenu* pMenu = CCMenu::menuWithItems(pCloseItem, NULL);
	pMenu->setPosition( CCPointZero );
	this->addChild(pMenu, 1);
	
	background = CCSprite::spriteWithFile("background.png");
	background->setPosition(ccp(275,180));
	this->addChild(background, 0);
	
	CCSize size = CCDirector::sharedDirector()->getWinSize();
	player = CCSprite::spriteWithFile("death_sprite.png");
	
	// position the sprite on the center of the screen
	player->setPosition( ccp(150,100) );
	
	// add the sprite as a child to this layer
	this->addChild(player, 0);
	
    this->schedule( schedule_selector(HelloWorld::gameLogic), 1.0 );
	
	this->setIsTouchEnabled(true);
	
	_targets = new CCMutableArray<CCSprite*>;
	_projectiles = new CCMutableArray<CCSprite*>;
	
	this->schedule(schedule_selector(HelloWorld::update));
//	CCLOG("Loading music");
//	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("background-music-aac.caf");
	
	return true;
}

void HelloWorld::gameLogic(cocos2d::ccTime dt)
{
    this->addTarget();
}

void HelloWorld::addTarget()
{//
	
    target = CCSprite::spriteWithFile("Blob.png");
	target->setPosition(ccp(380,100));
	this->addChild(target,0);
	
	
//	CCSprite *target = CCSprite::spriteWithFile("Blob.png");
//	
//    // Determine where to spawn the target along the Y axis
//	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
//	int minY = target->getContentSize().height/2;
//	int maxY = winSize.height - target->getContentSize().height/2;
//	int rangeY = maxY - minY;
//	int actualY = (arc4random() % rangeY) + minY;
//	
//    // Create the target slightly off-screen along the right edge,
//    // and along a random position along the Y axis as calculated above
//	target->setPosition(ccp(winSize.width + target->getContentSize().width/2, actualY));
//	this->addChild(target);
//	
//	// Determine speed of the target
//	int minDuration = 2.0;
//	//int maxDuration = 4.0;
//	int rangeDuration = minDuration;
//	int actualDuration = (arc4random() % rangeDuration) + minDuration;
//	
//    // Create the actions
//	CCFiniteTimeAction *actionMove = CCMoveTo::actionWithDuration(actualDuration, ccp(-target->getContentSize().width/2, actualY));
//	CCFiniteTimeAction *actionMoveDone = CCCallFuncN::actionWithTarget(this, callfuncN_selector(HelloWorld::spriteMoveFinished));
//    
	target->setTag(1);
	_targets->addObject(target);
//	
//    target->runAction(CCSequence::actions(actionMove, actionMoveDone, NULL));
}

bool haveCollided(int player_x,int player_y,int target_x,int target_y){
	player_x = player->getPosition().x;
	player_y = player->getPosition().y;
	target_x = target->getPosition().x;
	target_y = target->getPosition().y;
	
	if ((player_x + 55) == (target_x -55) || (player_x - 55) == (target_x + 55)) {
		return true;
	}else {
		return false;
	}

}

void HelloWorld::spriteMoveFinished(CCNode* sender)
{
    CCSprite *sprite = (CCSprite *)sender;
    this->removeChild(sprite, true);
	if (sprite->getTag() == 1) 
	{ // target
		_targets->removeObject(sprite);
	//	GameOverScene *gameOverScene = GameOverScene::node();
//		gameOverScene->getLayer()->getLabel()->setString("You Lose! :[");
//		CCDirector::sharedDirector()->replaceScene(gameOverScene);
		
	} 
	else if (sprite->getTag() == 2) 
	{ // projectile
		_projectiles->removeObject(sprite);
	}
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
	CCDirector::sharedDirector()->end();
	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif
}

//checks if the click is on the player sprite
bool isClickOnPlayer(int x, int y){
	int playerX_rb = player->getPosition().x + 35;
	int playerX_lb = player->getPosition().x - 35;
	int playerY_ub = player->getPosition().y +75;
	int playerY_lb = player->getPosition().y -75;
	
	if ((x >= playerX_lb && x <= playerX_rb) && (y <= playerY_ub && y >= playerY_lb)){
		return true;
	}else {
		return false;
	}
	
}

void HelloWorld::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent)
{
	CCTouch *touch = (CCTouch *)pTouches->anyObject();
	CCPoint location = touch->locationInView(touch->view());
	location = CCDirector::sharedDirector()->convertToGL(location);
	
	int bg_x = background->getPosition().x;
	int bg_y = background->getPosition().y;
	int player_x = player->getPosition().x;
	int player_y = player->getPosition().y;
	
	CCPoint realDest = ccp(location.x, player_y);
	
	float length = location.x - player_x;
	float velocity = 100/1; // 100pixel/sec
	float realMoveDuration = length/velocity;
	
	
	// player moves to the right if click on death's right-hand side
	if (!isClickOnPlayer(location.x, location.y) && location.x > player_x && player_x < 480 ) {
		player->setFlipX(false);
		player->runAction(CCSequence::actions(CCMoveTo::actionWithDuration(realMoveDuration, realDest),  NULL));
	}
	
	// player moves to the left if click on death's left-hand side
	if (!isClickOnPlayer(location.x, location.y) && location.x < player_x && player_x >0) {
		player->setFlipX(true);
		player->runAction(CCSequence::actions(CCMoveTo::actionWithDuration(realMoveDuration*(-1), realDest),  NULL));
	}
					  
}



void HelloWorld::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent)
{
	CCTouch *touch = (CCTouch *)pTouches->anyObject();
	CCPoint location = touch->locationInView(touch->view());
	location = CCDirector::sharedDirector()->convertToGL(location);
	
	// Set up initial location of projectile
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCSprite *projectile = CCSprite::spriteWithFile("ice_magic.png");
	projectile->setPosition(ccp(player->getPosition().x, player->getPosition().y));
	
	// Determine offset of location to projectile
	int offX = location.x - projectile->getPosition().x;
	int offY = location.y - projectile->getPosition().y;
	
	// Bail out if we are shooting down or backwards
	//if (offX <= 0) return;
	
	// Ok to add now - we've double checked position
	
	// add child if click on the player 
	if (isClickOnPlayer(location.x, location.y)){
	
	projectile->setFlipX(player->isFlipX());
	this->addChild(projectile);
	}
	
	
	// Determine where we wish to shoot the projectile to
	int realX = winSize.width + (projectile->getContentSize().width/2);
	float ratio = (float) offY / (float) offX;
	
	
	CCPoint realDest;
	if (player->isFlipX()){
	realDest = ccp(player->getPosition().x - 300,player->getPosition().y);
	}else{
	realDest = ccp(player->getPosition().x+ 300,player->getPosition().y);
	}
	
	// Determine the length of how far we're shooting
	float length = 200;
	float velocity = 320/1; // 480pixels/1sec
	float realMoveDuration = length/velocity;
	

	projectile->setTag(2);
	_projectiles->addObject(projectile);
	
	
	// Move projectile to actual endpoint
	projectile->runAction(CCSequence::actions(CCMoveTo::actionWithDuration(realMoveDuration, realDest), 
											  CCCallFuncN::actionWithTarget(this, callfuncN_selector(HelloWorld::spriteMoveFinished)),
											  NULL));
	
	//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("pew-pew-lei.caf");
	
}

void HelloWorld::update(cocos2d::ccTime dt)
{
	CCMutableArray<cocos2d::CCSprite*> *projectilesToDelete = new CCMutableArray<CCSprite *>;
	
	CCMutableArray<CCSprite*>::CCMutableArrayIterator it, jt;
	
	for (it = _projectiles->begin(); it != _projectiles->end(); it++)
	{
		CCSprite *projectile = *it;
		CCRect projectileRect = CCRectMake(
										   projectile->getPosition().x - (projectile->getContentSize().width/2), 
										   projectile->getPosition().y - (projectile->getContentSize().height/2), 
										   projectile->getContentSize().width, 
										   projectile->getContentSize().height);
		CCMutableArray<cocos2d::CCSprite*> *targetsToDelete = new CCMutableArray<cocos2d::CCSprite*>;
		for (jt = _targets->begin(); jt != _targets->end(); jt++)
		{
			CCSprite *target = *jt;
			CCRect targetRect = CCRectMake(
										   target->getPosition().x - (target->getContentSize().width/2), 
										   target->getPosition().y - (target->getContentSize().height/2), 
										   target->getContentSize().width, 
										   target->getContentSize().height);
			
			if (CCRect::CCRectIntersectsRect(projectileRect, targetRect)) {
				targetsToDelete->addObject(target);
			}
		}
		
		for (jt = targetsToDelete->begin(); jt != targetsToDelete->end(); jt++) 
		{
			_targets->removeObject(*jt);
			this->removeChild((*jt), true);
			
			_projectilesDestroyed++;
			
			//if (_projectilesDestroyed > 30) {
//				GameOverScene *gameOverScene = GameOverScene::node();
//				_projectilesDestroyed = 0;
//				gameOverScene->getLayer()->getLabel()->setString("You Win!");
//				CCDirector::sharedDirector()->replaceScene(gameOverScene);
//			}
		}
		
		if (targetsToDelete->count())
		{
			projectilesToDelete->addObject(*it);
		}
		
		targetsToDelete->release();
	}
	
	for (it = projectilesToDelete->begin(); it != projectilesToDelete->end(); it++)
	{
		_projectiles->removeObject(*it);
		this->removeChild(*it, true);
	}
	projectilesToDelete->release();
}
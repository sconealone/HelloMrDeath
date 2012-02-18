#include "HelloWorldScene.h"
using namespace cocos2d;

CCScene* HelloWorld::scene()
{
    CCScene * scene = NULL;
    do 
    {
        // 'scene' is an autorelease object
        scene = CCScene::node();
        CC_BREAK_IF(! scene);

        // 'layer' is an autorelease object
        HelloWorld *layer = HelloWorld::node();
        CC_BREAK_IF(! layer);

        // add layer as a child to scene
        scene->addChild(layer);
    } while (0);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{	
	CocosDenshion::SimpleAudioEngine::sharedEngine()->playBackgroundMusic("nyan cat.mid", true);
    bool bRet = false;
    do 
    {
        //////////////////////////////////////////////////////////////////////////
        // super init first
        //////////////////////////////////////////////////////////////////////////

        CC_BREAK_IF(! CCLayer::init());

        //////////////////////////////////////////////////////////////////////////
        // add your codes below...
        //////////////////////////////////////////////////////////////////////////

        // 1. Add a menu item with "X" image, which is clicked to quit the program.

        // Create a "close" menu item with close icon, it's an auto release object.
        CCMenuItemImage *pCloseItem = CCMenuItemImage::itemFromNormalImage(
            "CloseNormal.png",
            "CloseSelected.png",
            this,
            menu_selector(HelloWorld::menuCloseCallback));
        CC_BREAK_IF(! pCloseItem);

        // Place the menu item bottom-right conner.
        pCloseItem->setPosition(ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20));

        // Create a menu with the "close" menu item, it's an auto release object.
        CCMenu* pMenu = CCMenu::menuWithItems(pCloseItem, NULL);
        pMenu->setPosition(CCPointZero);
        CC_BREAK_IF(! pMenu);

        // Add the menu to HelloWorld layer as a child layer.
		pMenu->setTag(MENU_TAG);
        this->addChild(pMenu, 1);

		// Create the main character sprite
		const float DEATH_SPRITE_WIDTH = 150;
		const float DEATH_SPRITE_HEIGHT = 150;
		CCSprite* deathSprite = CCSprite::spriteWithFile("death_sprite.png", CCRect(0, 0, DEATH_SPRITE_WIDTH, DEATH_SPRITE_HEIGHT));
		CCSize windowSize = CCDirector::sharedDirector()->getWinSize();
		const int GROUND_OFFSET = 50;
		// the coordinates start from the bottom left corner of the sprite
		// If I want the centre of Mr. Death to start at the first third of the screen, then I need to offset his sprite to the left by half of its width.
		deathSprite->setPosition(ccp(windowSize.width / 3 - deathSprite->getContentSize().width / 2, deathSprite->getContentSize().height / 2 + GROUND_OFFSET));
		this->addChild(deathSprite, 2);  // higher z index means closer to screen?
		mrDeathSprite = deathSprite;

		CCSprite* deathAttackSprite = CCSprite::spriteWithFile("death_attack.png", CCRect(0,0, 200, 150));
		CCPoint mrDeathCentre = deathSprite->getPosition();
		int attackSpriteCentreXCoord = mrDeathCentre.x + 25;
		deathAttackSprite->setPosition(ccp(attackSpriteCentreXCoord, mrDeathCentre.y));
		deathAttackSprite->setIsVisible(false);
		this->addChild(deathAttackSprite, 2);
		deathAttackSprite->setTag(ATTACK_TAG);
		mrDeathAttackingSprite = deathAttackSprite;


		/*
		// create background
		const float BACKGROUND_WIDTH = 1977;
		const float BACKGROUND_HEIGHT = 780;
		CCSprite* background = CCSprite::spriteWithFile("test_background.png", CCRect(0, 0, BACKGROUND_WIDTH, BACKGROUND_HEIGHT));
		background->setPosition(ccp(background->getContentSize().width / 2, windowSize.height / 2));
		this->addChild(background, 0);
		
		// create floor
		CCSprite* floor = CCSprite::spriteWithFile("floor.png", CCRect(0,0,1977, 50));
		floor->setPosition(ccp(floor->getContentSize().width / 2, floor->getContentSize().height / 2));
		this->addChild(floor,1);
		*/
        bRet = true;
    } while (0);

	_watermelons = new CCMutableArray<CCSprite*>;

	// Setting the background as part of the Tiled Map Editor test
	
	tileMap = CCTMXTiledMap::tiledMapWithTMXFile("test_tile_map.tmx");
	tiledBg = tileMap->layerNamed("Background");
	
	this->addChild(tileMap, -1);

	// not entirely sure what an object group is yet
	CCTMXObjectGroup *tiledMapObjects = tileMap->objectGroupNamed("Objects");
	if (tiledMapObjects == NULL)
	{
		bRet = false;
	}

	// a dictionary is like a hashtable
	// A CCStringToStringDictionary is the same as CCMutableDictionary<string,CCString*>
	CCMutableDictionary<string, CCString*> *spawnPoint = tiledMapObjects->objectNamed("BlobSpawnPoint");
	if (spawnPoint == NULL)
	{
		bRet = false;
	}

	int blobX = spawnPoint->objectForKey("x")->toInt();
	int blobY = spawnPoint->objectForKey("y")->toInt();

	// Create the blob as part of the Tiled Map Editor test
	blobSprite = CCSprite::spriteWithFile("Blob.png", CCRect(0, 0, 150, 150));
	blobSprite->setPosition(ccp( (float) blobX, (float) blobY ));
	this->addChild(blobSprite, 2);
	CCSprite* deathSprite = mrDeathSprite;
	setViewpointCentre();


	
	this->setIsTouchEnabled(true);
	this->schedule(schedule_selector(HelloWorld::gameLoop), 1.0);
	this->schedule(schedule_selector(HelloWorld::update));
	this->schedule(schedule_selector(HelloWorld::checkKeyboardInput, 0.2f));


    return bRet;
}

HelloWorld::HelloWorld():_watermelons(NULL){
	isTouchingMrDeath = false;
	isMrDeathAttacking = false;
	isMrDeathMoving = false;
	isMrDeathFacingRight = true;
	mrDeathSprite = NULL;
	mrDeathAttackingSprite = NULL;
	blobSprite = NULL;
	tileMap = NULL;
	tiledBg = NULL;
}

HelloWorld::~HelloWorld()
{
	_watermelons->release();
	_watermelons = NULL;
}

void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    // "close" menu item clicked
    CCDirector::sharedDirector()->end();
}


/*
	Adds a placeholder enemy to the scene
*/
void HelloWorld::addTarget()
{
	// Create the enemy.  Is the rectangle the enemy's hitbox?  Is the x,y of the rectangle the centre?
	// Tests indicate that x, y is actually the centre of the rect!
	// The height and width controls how much of the sprite to show, but doesn't scale the image
	CCSprite *enemy = CCSprite::spriteWithFile("watermelon.png", CCRectMake(0,0,150,150));

	// the enemy will spawn just offscreen 
	CCSize windowSize = CCDirector::sharedDirector()->getWinSize();
	const int GROUND_OFFSET = 50; // should make this a class-wide constant
	enemy->setPosition(ccp(tileMap->getMapSize().width*tileMap->getTileSize().width + enemy->getContentSize().width, enemy->getContentSize().height/ 2 + GROUND_OFFSET));
	enemy->setTag(WATERMELON_TAG);
	_watermelons->addObject(enemy);
	this->addChild(enemy);

	// random duration
	int minDuration = 2;
	int maxDuration = 4;
	int rangeDuration = maxDuration - minDuration;
	int actualDuration = (rand() % rangeDuration) + minDuration;
	
	// create the action of moving
	CCPoint moveToPoint = ccp(0 - enemy->getContentSize().width / 2, enemy->getContentSize().height / 2 + GROUND_OFFSET);
	CCFiniteTimeAction *actionMove = CCMoveTo::actionWithDuration( (ccTime) actualDuration, moveToPoint);
	
	// signature: actionWithTarget (SelectorProtocol *pSelectorTarget, SEL_CallFunc selector)
	// CCLayer (which this is) inherits from SelectorProtocol, and SEL_CallFunc I guess will just be a function from this class
	CCFiniteTimeAction *actionMoveDone = CCCallFuncN::actionWithTarget(this, callfuncN_selector(HelloWorld::spriteMoveFinished));

	// runs a sequence of actions?  Uses pointers to CCFiniteTimeAction objects, with NULL being used to mark the end of the sequence
	enemy->runAction(CCSequence::actions(actionMove, actionMoveDone, NULL));
	
}



/**
	Says what to do when the sprite move is finished?
	Will remove the sprite from the screen?
*/
void HelloWorld::spriteMoveFinished(CCNode* sender)
{
	CCSprite *sprite = (CCSprite*) sender;
	// remove the sending sprite and clean up

	if (sprite->getTag() == WATERMELON_TAG)
	{
		_watermelons->removeObject(sprite);
		this->removeChild(sprite, true);
	}
}


/**
	The code that should run in our game loop?
	Scheduled to run every 1 ms I think, in a call from init.
*/
void HelloWorld::gameLoop(float dt)
{
	this->addTarget();
}



void HelloWorld::ccTouchesBegan(CCSet* pTouches, CCEvent* pEvent)
{
	CCSprite* mrDeath = mrDeathSprite;

	// get the first touch location
	CCTouch* touch = (CCTouch*) pTouches->anyObject();
	CCPoint location = touch->locationInView(touch->view());

	// check if the touch is on mr. death
	CCPoint mrDeathCentre = mrDeath->getPosition();
	CCSize mrDeathSize = mrDeath->getContentSize();
	int horizontalLowerBound = mrDeathCentre.x - mrDeathSize.width / 2;
	int horizontalUpperBound = mrDeathCentre.x + mrDeathSize.width / 2;
	//int verticalLowerBound = CCDirector::sharedDirector()->getWinSize().height - mrDeathSize.height - mrDeathCentre.y- mrDeathSize.height/2;
	int verticalUpperBound = CCDirector::sharedDirector()->getWinSize().height - mrDeathCentre.y+ mrDeathSize.height/2;
	int verticalLowerBound = verticalUpperBound - mrDeathSize.height;
	if (	location.x >= horizontalLowerBound
		&&	location.x <= horizontalUpperBound
		&&	location.y >= verticalLowerBound
		&&	location.y <= verticalUpperBound)
	{

		// if yes, replace mr. death with mr. death attacking
		isTouchingMrDeath = true;
		CCSprite* mrDeathAttacks = mrDeathAttackingSprite;
		showMrDeathAttacking();
		
	}

	else if (	location.x < horizontalLowerBound
				&& location.y <= verticalUpperBound)
				// if touch is left of mr. death's centre, but not above him, move him left
	{
		moveMrDeath(false);
	}

	// if the touch is right of mr. death and not above him move him right
	else if (location.x > horizontalUpperBound && location.y <= verticalUpperBound)
	{
		moveMrDeath(true);
	}

	// if the touch is above mr. death then jump
	else if (location.y > verticalUpperBound)
	{
		mrDeathJump();
	}

	// otherwise do nothing
}

void HelloWorld::deathDidFinishMoving(CCNode* sender)
{
	isMrDeathMoving  = false;
}

void HelloWorld::moveMrDeath(bool forward)
{
	if (!isMrDeathMoving 
		&& (	(mrDeathSprite->getPosition().x - mrDeathSprite->getContentSize().width/2 >= 0 && !forward)
			//||	(mrDeathSprite->getPosition().x + mrDeathSprite->getContentSize().width/2 <= CCDirector::sharedDirector()->getWinSize().width && forward)	))
			||	(	(mrDeathSprite->getPosition().x + mrDeathSprite->getContentSize().width/2 
						<= tileMap->getTileSize().width*tileMap->getMapSize().width)
						&& forward)	))
	{
		if (isMrDeathFacingRight && !forward)
		{
			isMrDeathFacingRight = false;
			CCFiniteTimeAction* flipNormal = CCFlipX::actionWithFlipX(true);
			CCFiniteTimeAction* flipAttacking = CCFlipX::actionWithFlipX(true);
			mrDeathSprite->runAction(flipNormal);
			mrDeathAttackingSprite->runAction(flipAttacking);
		}
		else if (!isMrDeathFacingRight && forward)
		{
			isMrDeathFacingRight = true;
			CCFiniteTimeAction* flipNormal = CCFlipX::actionWithFlipX(false);
			CCFiniteTimeAction* flipAttacking = CCFlipX::actionWithFlipX(false);
			mrDeathSprite->runAction(flipNormal);
			mrDeathAttackingSprite->runAction(flipAttacking);
		}

		int directionMultiplier = (forward) ? 1 : -1;
		CCPoint targetPoint = ccp(directionMultiplier* mrDeathSprite->getContentSize().width/8,
								0);
		const float MOVE_TIME = 0.01f; // unit is seconds
		CCFiniteTimeAction* move = CCMoveBy::actionWithDuration((ccTime) MOVE_TIME,targetPoint);
		CCFiniteTimeAction* finishedMove = CCCallFuncN::actionWithTarget(this, callfuncN_selector(HelloWorld::deathDidFinishMoving));
		CCFiniteTimeAction* youAlsoMoveYouSillyAttackSprite = CCMoveBy::actionWithDuration((ccTime) MOVE_TIME, targetPoint);
		CCFiniteTimeAction* finishedAttackingMove = CCCallFuncN::actionWithTarget(this, callfuncN_selector(HelloWorld::deathDidFinishMoving));
		mrDeathSprite->runAction(CCSequence::actions(move, finishedMove, NULL));
		mrDeathAttackingSprite->runAction(CCSequence::actions(youAlsoMoveYouSillyAttackSprite, finishedAttackingMove, NULL));
		isMrDeathMoving = true;

		setViewpointCentre();
	}
}




void HelloWorld::mrDeathJump()
{

}



void HelloWorld::swapSprites(CCSprite* spriteToMakeInvisible, CCSprite* spriteToMakeVisible)
{
	spriteToMakeInvisible->setIsVisible(false);
	spriteToMakeVisible->setIsVisible(true);
}


void HelloWorld::showMrDeathAttacking()
{
	CCSprite* _mrDeathSprite = mrDeathSprite;
	CCSprite* _mrDeathAttackingSprite = mrDeathAttackingSprite;
	swapSprites(_mrDeathSprite, _mrDeathAttackingSprite);
	if (!isMrDeathAttacking)
	{
		CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("smb_stomp.wav");
	}
	isMrDeathAttacking = true;
}

void HelloWorld::showMrDeathNormal()
{
	CCSprite* _mrDeathSprite = mrDeathSprite;
	CCSprite* _mrDeathAttackingSprite = mrDeathAttackingSprite;
	swapSprites(_mrDeathAttackingSprite, _mrDeathSprite);
	isMrDeathAttacking = false;
}

void HelloWorld::ccTouchesEnded(CCSet* pTouches, CCEvent* pEvent)
{
	showMrDeathNormal();
	isTouchingMrDeath = false;
}



void HelloWorld::update(float dt)
{
	// check if watermelons collided with an attacking sprite
	// check all melons, add them to a list to delete if they collide with an attacking sprite
	CCMutableArray<CCSprite*> *melonsToDelete = new CCMutableArray<CCSprite*>;
	CCMutableArray<CCSprite*>::CCMutableArrayIterator melonIterator;
	bool effectWasPlayed = false;
	for (melonIterator = _watermelons->begin(); melonIterator != _watermelons->end(); melonIterator++)
	{
		CCSprite *melon = *melonIterator;
		CCRect melonBoundary = CCRectMake(
			melon->getPosition().x - melon->getContentSize().width / 2, // does rect make make from the bottom left corner? 
			melon->getPosition().y - melon->getContentSize().height / 2,
			melon->getContentSize().width,
			melon->getContentSize().height);
		CCRect attackSpriteBoundary = CCRectMake(
			mrDeathAttackingSprite->getPosition().x - mrDeathAttackingSprite->getContentSize().width/2,
			mrDeathAttackingSprite->getPosition().y - mrDeathAttackingSprite->getContentSize().height/2,
			mrDeathAttackingSprite->getContentSize().width,
			mrDeathAttackingSprite->getContentSize().height);
		if (mrDeathAttackingSprite->getIsVisible() && CCRect::CCRectIntersectsRect(melonBoundary, attackSpriteBoundary))
		{
			melonsToDelete->addObject(melon);
			if (!effectWasPlayed)
			{
				//CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect("dragpunch.wav");
				effectWasPlayed = true;
			}
		}
	}

	for (melonIterator = melonsToDelete->begin(); melonIterator != melonsToDelete->end(); melonIterator++)
	{
		CCSprite* melon = *melonIterator;
		_watermelons->removeObject(melon);
		this->removeChild(melon, true);
	}
	melonsToDelete->release();
}


void HelloWorld::checkKeyboardInput(float dt)
{
	
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	
	// keyboard test
	const int ATTACK_KEY = 0x45; // 0x45 is the E key
	if (GetAsyncKeyState(ATTACK_KEY))
	{
		//swapSprites(mrDeathSprite, mrDeathAttackingSprite);
		showMrDeathAttacking();
	}
	else if (!isTouchingMrDeath)
	{
		//swapSprites(mrDeathAttackingSprite, mrDeathSprite);
		showMrDeathNormal();
	}

	const int MOVE_LEFT = 0x41;
	int wasLeftPressed = GetAsyncKeyState(MOVE_LEFT);
	if (wasLeftPressed)
	{
		moveMrDeath(false);
	}
	const int MOVE_RIGHT = 0x44;
	if (GetAsyncKeyState(MOVE_RIGHT))
	{
		moveMrDeath(true);
	}

	#endif
}

/**
	Tiled Map Editor test
	Sets the view to focus on the point.
	In general it would be where the player (Mr. Death) is.
	Translated from the Objective C tutorial 
	http://www.raywenderlich.com/1163/how-to-make-a-tile-based-game-with-cocos2d

	for the actual project, this needs to be modified to take into account that Mr. Death moves left
	So we will need to do a camera shift when he goes left.

	Also this will cause issues for the display of the menu.
	We can't simply move the entire scene, unless it's displayed separatedly than the UI layer.

	Also, it's really screwing with the touch coordinates
*/
void HelloWorld::setViewpointCentre()
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();

	// This part sets limmits on how far the camera will follow when you hit the beginning and ends of the level

	// Makes it so that when you start, the window is centred, but when you run to the centre, the camera starts following you
	float deathXOffsetFromCentre = mrDeathSprite->getContentSize().width/2 + mrDeathSprite->getPosition().x + winSize.width/6;
	float x = max(winSize.width/2, deathXOffsetFromCentre);
	float y = winSize.height/2; // needs to be changed when jump gets implemented
	// mapsize*tilesize gives the size of the map in pixels
	// makes it so that when you get to the end of the tilemap, the camera stays in the centre, but follows you otherwise
	x = min(x, tileMap->getMapSize().width*tileMap->getTileSize().width - winSize.width/2);
	// add y when jump gets implemented

	

	CCPoint actualPosition = ccp(x, y);
	CCPoint centreOfView = ccp(winSize.width/2, winSize.height/2);
	CCPoint viewPoint = ccpSub(centreOfView, actualPosition);
	CCNode* pMenu = this->getChildByTag(MENU_TAG);
	float dx = this->getPosition().x - viewPoint.x;
	float dy = this->getPosition().y - viewPoint.y;
	pMenu->setPosition(ccp(pMenu->getPosition().x + dx, pMenu->getPosition().y + dy));
	this->setPosition(viewPoint);
}

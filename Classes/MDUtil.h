#pragma once
#ifndef MDUTIL_H
#define MDUTIL_H
#include "Box2D.h"
#include "cocos2d.h"
#define TICK_TIME 1.0f / 60.0f // 60 frames per second
class MDUtil {
public:
	
	
	/**
	Converts tiles to metres, according to the game world.
	*/
	static float tilesToMetres(float tiles);

	/**
	Converts pixels ot metres, according to the game world.
	*/
	static float pixelsToMetres(float px);

	static float metresToPixels(float m);
	static b2Vec2 toB2Vec2(cocos2d::CCPoint point);
	static cocos2d::CCPoint toCCPoint(b2Vec2 vec);

	static const int PIXELS_PER_TILE = 32;
	static const int PIXELS_PER_METRE = 32; // placeholder

};
#endif
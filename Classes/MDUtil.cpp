#include "MDUtil.h"
using namespace cocos2d;
/**
 * Converts a size in tiles to a size in metres
 */
float MDUtil::tilesToMetres(float tileNumber) {
	return tileNumber * PIXELS_PER_TILE / PIXELS_PER_METRE;
}

float MDUtil::pixelsToMetres(float px) {
	return px / PIXELS_PER_METRE;
}

float MDUtil::metresToPixels(float m) {
	return m * PIXELS_PER_METRE;
}

b2Vec2 MDUtil::toB2Vec2(CCPoint point) {
	return b2Vec2(pixelsToMetres(point.x), pixelsToMetres(point.y));
}
CCPoint MDUtil::toCCPoint(b2Vec2 vec) {
	return ccp(metresToPixels(vec.x), metresToPixels(vec.y));
}
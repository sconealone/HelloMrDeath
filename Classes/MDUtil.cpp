#include "MDUtil.h"

/**
 * Converts a size in tiles to a size in metres
 */
float MDUtil::tilesToMetres(float tileNumber) {
	return tileNumber * PIXELS_PER_TILE / PIXELS_PER_METRE;
}

float MDUtil::pixelsToMetres(float px) {
	return px / PIXELS_PER_METRE;
}

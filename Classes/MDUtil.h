#pragma once
#ifndef MDUTIL_H
#define MDUTIL_H
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

	static const int PIXELS_PER_TILE = 32;
	static const int PIXELS_PER_METRE = 64; // placeholder
};
#endif
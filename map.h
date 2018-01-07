#ifndef MAP_H
#define MAP_H

#include "game.h"
#include "object.h"

//#include <list>

//TODO: Use screen constants for this:
#define ZONE_WIDTH 1280
#define ZONE_HEIGHT 800
#define NUM_ZONES 50
#define MAP_HEIGHT NUM_ZONES * ZONE_HEIGHT 

class Map {
public:
	Map();
	void render();
	void renderZone(int zoneIndex);
	void renderObject(Object& obj);
	bool collidedWith(Object& obj);
	bool collidedWithZone(Object& obj, int zoneIndexc);
	int getZoneIndex(long worldY);
	long zoneWidth;
	long zoneHeight;
	long screenY;
	//std::list<Object> objects;
	//std::list<Object> objects_zoned[NUM_ZONES];
};

#endif

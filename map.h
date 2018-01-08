#ifndef MAP_H
#define MAP_H

#include "game.h"
#include "object.h"

//TODO: Use screen constants for this:
#define ZONE_WIDTH 1280
#define ZONE_HEIGHT 800
#define NUM_ZONES 50
#define MAP_HEIGHT NUM_ZONES * ZONE_HEIGHT 

class Map {
public:
	Map();
        bool initialize();
	void render();
	void renderZone(int zoneIndex);
	void renderObject(Object& obj);
	bool collidedWith(Object& obj);
	bool collidedWithZone(Object& obj, int zoneIndexc);
	int getZoneIndex(long worldY);
	long screenY;
        std::list<Object*> objs;
	std::array<std::list<Object>,NUM_ZONES> objs_zoned;
};

#endif

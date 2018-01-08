#ifndef MAP_H
#define MAP_H

#include "game.h"
#include "object.h"

//TODO: Use screen constants for this:
#define ZONE_WIDTH 1280
#define ZONE_HEIGHT 800
#define NUM_ZONES 50
#define MAP_HEIGHT NUM_ZONES * ZONE_HEIGHT 

class Object;

class Map {
public:
	Map();
        bool initialize();
	void render();
	void renderZone(int zoneIndex);
	void renderBitmap(ALLEGRO_BITMAP *bitmap, long x, long y);
	bool collidedWith(Object& obj);
	bool collidedWithZone(Object& obj, int zoneIndexc);
	int getZoneIndex(long worldY);
	long screenY;
        std::list<Object*> objs;
	std::array<std::list<Object>,NUM_ZONES> objs_zoned;
};

#endif

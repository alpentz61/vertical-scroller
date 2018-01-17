#ifndef MAP_H
#define MAP_H

#include "game.h"
#include "object.h"

//TODO: Use screen constants for this:
#define ZONE_WIDTH 1280
#define ZONE_HEIGHT 800
#define NUM_ZONES 10
#define MAP_HEIGHT NUM_ZONES * ZONE_HEIGHT

class Object;

struct Config {
	long x;
	long y;
  std::string object_type;
	std::string object_label;
};

class Map {
public:
	Map();
  bool initialize();
	bool loadResources();
	//bool loadConfig(const std::string& cfg);
	void render();
	void renderZone(int zoneIndex);
	void renderBitmap(ALLEGRO_BITMAP *bitmap, long x, long y, long height);
	void animate();
	void animateZone(int zoneIndex);
	bool collidedWith(Object& obj);
	bool collidedWithZone(Object& obj, int zoneIndexc);
	int getZoneIndex(long worldY);
	long screenY;
	std::list<Config> config; //TODO: For polymorphism to work, these must be Object pointers
  std::list<Object*> objs;
	std::array<std::list<Object*>,NUM_ZONES> objs_zoned;
	ALLEGRO_FONT *font;
	ALLEGRO_BITMAP *honeypot_bitmap;
	ALLEGRO_BITMAP *switch_bitmap;
	ALLEGRO_BITMAP *firewall_bitmap;
	ALLEGRO_BITMAP *scanner_bitmap;


};

#endif

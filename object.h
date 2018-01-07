#ifndef OBJECT_H
#define OBJECT_H

#include "game.h"

class Object {
public:
	Object();
	void render();
	void setBitmapHwnd(ALLEGRO_BITMAP *bmp_handle);
	ALLEGRO_BITMAP *bmp_handle;
	long x;
	long y;
	long width;
	long height;
};


#endif

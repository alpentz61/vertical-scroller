#ifndef OBJECT_H
#define OBJECT_H

#include "game.h"
#include "map.h"

class Map;
class Object;

typedef enum{
   OBJECT,
   SWITCH_PASS,
   SWITCH_KILL
}CollisionType;

class Collision{
public:
   Object *collObject;
   CollisionType collType;
};

class Object {
public:
   Object();
   virtual void render();
   virtual bool collidedWith(Object *other);
   virtual void animate();
   void setBitmapHwnd(ALLEGRO_BITMAP *bmp_handle);
   void setCollision(Collision& collision);
   void clearCollisions();
   Map *map;	
   ALLEGRO_BITMAP *bmp_handle;
   long x;
   long y;
   long width;
   long height;
   std::list<Collision> collList;
};

class Switch : public Object {
public:
   Switch();
   virtual void render();
   virtual bool collidedWith(Object *other);
   virtual void animate();
   ALLEGRO_FONT *font;
   unsigned int code;
   char codeString[4];
   bool updateLabel;
   int counter;

};

#endif

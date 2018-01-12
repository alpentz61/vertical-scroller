#ifndef OBJECT_H
#define OBJECT_H

#include "game.h"
#include "map.h"

class Map;
class Object;
class Switch;
class Honeypot;

typedef enum{
   OBJECT,
   SWITCH_PASS,
   SWITCH_KILL
}CollisionType;

class Vector {
public:
  float x;
  float y;
  float magnitude();
  bool normalize();
};

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

class Player : public Object {
public:
   Player();
   virtual void animate();
   void honeypotCatch(Honeypot *pot);
   bool isKilled();
   void kill();
   bool moveUp();
   bool moveDown();
   bool moveLeft();
   bool moveRight();
   void setXLocked(bool locked);
   bool xLocked;
   bool killed;
   bool isCaught;
   int chaseTimer;
   Honeypot *honeypot;
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

class Honeypot : public Object {

};

#endif

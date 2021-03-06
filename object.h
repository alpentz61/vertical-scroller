#ifndef OBJECT_H
#define OBJECT_H

#include "game.h"
#include "map.h"

#define DETECT_DIST 200.0
const int SCREEN_W = 800;//TODO: remove constant duplication

class Map;
class Object;
class Switch;
class Honeypot;

typedef enum{
   OBJECT,
   SWITCH_PASS,
   SWITCH_KILL,
   HONEYPOT_DETECT,
   HONEYPOT_KILL,
   SCANNER_TRACK,
   SCANNER_KILL
}CollisionType;
typedef enum{
  PLAYER,
  SWITCH,
  SCANNER,
  HONEYPOT,
  FIREWALL,
  FLAG
}ObjectType;

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
   Object(ObjectType objType_);
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
   ObjectType objType;
};

class Player : public Object {
public:
   Player();
   virtual void animate();
   void honeypotCatch(Honeypot *pot);
   bool isKilled();
   void kill();
   bool hasWon();
   void win();
   bool moveUp();
   bool moveDown();
   bool moveLeft();
   bool moveRight();
   void setXLocked(bool locked);
   bool xLocked;
   bool killed;
   bool won;
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

class Scanner : public Object {
public:
  Scanner();
  virtual bool collidedWith(Object *other);
  virtual void animate();
  void trackPlayer(Player *player);
  bool isTracking;
  Player *player;
  int chaseTimer;
  int moveSpeed;
};

class Honeypot : public Object {
public:
  Honeypot();
  virtual bool collidedWith(Object *other);
};

class Firewall : public Object {
public:
  Firewall();
  virtual void render();
  virtual bool collidedWith(Object *other);
  virtual void animate();
  long x1_disp;
  long y1_disp;
  long x2_disp;
  long y2_disp;
  bool movingOut;
};

class Flag : public Object {
public:
  Flag();
  virtual bool collidedWith(Object *other);
};

#endif

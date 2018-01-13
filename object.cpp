#include "object.h"

float Vector::magnitude(){
  float mag = sqrt(x*x + y*y);
}
bool Vector::normalize(){
  float mag = magnitude();
  if (mag > 0){
    x=x/mag;
    y=y/mag;
  }
}

Object::Object():bmp_handle(NULL),map(NULL),x(0),y(0),width(0),height(0){}
void Object::render(){
   if (bmp_handle == NULL){
     std::cerr << "Handle is null\n";
   }
   map->renderBitmap(bmp_handle, x, y);
}
bool Object::collidedWith(Object *other){
   if (x < (other->x + other->width) &&
       (x + width) > other->x        &&
       y < (other->y + other->height) &&
       (y + height) > other->y
       ){

      Collision collision;
      collision.collObject=this;
      collision.collType=OBJECT;
      other->setCollision(collision);
      return true;
   }
   return false;
}
void Object::animate(){
//animate does nothing by default
}
void Object::setBitmapHwnd(ALLEGRO_BITMAP *bmp_handle_){
   bmp_handle = bmp_handle_;
   width = al_get_bitmap_width(bmp_handle);
   height = al_get_bitmap_height(bmp_handle);
}
void Object::setCollision(Collision& collision){
   collList.push_back(collision);
}
void Object::clearCollisions(){
   collList.clear();
}

Player::Player():xLocked(false),killed(false),isCaught(false){}
void Player::animate(){
   if (isCaught){
      //Move towards the honeypot
      Vector vect;
      vect.x = honeypot->x - x;
      vect.y = honeypot->y - y;
      vect.normalize();
      vect.x *= 4;
      vect.y *= 4;
      x += vect.x;
      y += vect.y;
      //Move for duration specified by chase timer
      //This should be close enough to kill the player
      chaseTimer--;
      if (chaseTimer <= 0){
        isCaught = false;
      }
   }
}
void Player::honeypotCatch(Honeypot *pot){
   honeypot = pot;
   isCaught = true;
   chaseTimer = 300; //Chase for 5 s.
}
bool Player::isKilled(){
  return killed;
}
void Player::kill(){
   killed = true;
}
bool Player::moveUp(){
   if (!isCaught){
      y += 4;
     return true;
   }
   else {
     return false;
   }
}
bool Player::moveDown(){
  if (!isCaught){
     y -= 4;
    return true;
  }
  else {
    return false;
  }
}
bool Player::moveLeft(){
  if (!isCaught && !xLocked){
     x -= 4;
    return true;
  }
  else {
    return false;
  }
}
bool Player::moveRight(){
  if (!isCaught && !xLocked){
     x += 4;
    return true;
  }
  else {
    return false;
  }
}
void Player::setXLocked(bool locked){
   xLocked = locked;
}

Switch::Switch():font(NULL),code(0),codeString("0"),updateLabel(false),counter(0){}
void Switch::render(){
   map->renderBitmap(bmp_handle, x, y);
   long renderY = ZONE_HEIGHT - ((-12+y)-map->screenY);
   al_draw_text(font, al_map_rgb(255,255,255), 372,renderY,0, codeString);
}
bool Switch::collidedWith(Object *other){
   long x_player = other->x;
   long x1_player = other->x + other->width;
   long y_player = other->y;
   long y1_player = other->y + other->height;
   long x_switch = x;
   long x1_switch = x + width;
   long y_switch = y;
   long y1_switch = y + height;
   if ((y1_player > y_switch && y1_player < y1_switch) ||
       (y_player > y_switch && y_player < y1_switch)) {
      Collision collision;
      collision.collObject=this;
      if ((x_player > 50) && (x1_player < 150)){
         if (!(code & 8)){
            collision.collType = SWITCH_PASS;
         }
         else {
            collision.collType = SWITCH_KILL;
         }
      }
      else if ((x_player > 250) && (x1_player < 350)){
         if (!(code & 4)){
            collision.collType = SWITCH_PASS;
         }
         else {
            collision.collType = SWITCH_KILL;
         }
      }
      else if ((x_player > 450) && (x1_player < 550)){
         if (!(code & 2)){
            collision.collType = SWITCH_PASS;
         }
         else {
            collision.collType = SWITCH_KILL;
         }
      }
      else if ((x_player > 650) && (x1_player < 750)){
         if (!(code & 1)){
            collision.collType = SWITCH_PASS;
         }
         else {
            collision.collType = SWITCH_KILL;
         }
      }
      else {
         collision.collType = SWITCH_KILL;
      }
      other->setCollision(collision);
      return true;
   }
   return false;
}
void Switch::animate(){
   counter++;
   if (counter == 120){
      counter = 0;
      sprintf(codeString,"%X",code);
      code++;
      if (code == 16){
         code = 0;
      }
   }
}

Scanner::Scanner():isTracking(false),player(NULL),chaseTimer(0),moveSpeed(4){}
bool Scanner::collidedWith(Object *other){
   //Detect kill collisions on impact
   if (x < (other->x + other->width) &&
      (x + width) > other->x        &&
      y < (other->y + other->height) &&
      (y + height) > other->y
      ){
      Collision collision;
      collision.collObject=this;
      collision.collType=SCANNER_KILL;
      other->setCollision(collision);
      return true;
   } else { //Start scanning when within a given distance
      Vector vect;
      vect.x = other->x - x;
      vect.y = other->y - y;
      float magnitude = vect.magnitude();
      if (magnitude < DETECT_DIST){
         Collision collision;
         collision.collObject=this;
         collision.collType=SCANNER_TRACK;
         other->setCollision(collision);
         return true;
     }
   }
   return false;
}
void Scanner::animate(){
  if (isTracking){
    Vector vect;
    vect.x = player->x - x;
    vect.y = player->y - y;
    vect.normalize();
    vect.x *= 4;
    vect.y *= 4;
    x += (long)vect.x;
    y += (long)vect.y;
    //Move for duration specified by chase timer
    //This should be close enough to kill the player
    chaseTimer--;
    if (chaseTimer <= 0){
      isTracking = false;
    }
  }
  else {
    x += moveSpeed;
    if (x > SCREEN_W-width){
      moveSpeed = -4;
    }
    if (x < 0){
      moveSpeed = 4;
    }
  }
}
void Scanner::trackPlayer(Player *player_){
  player = player_;
  isTracking = true;
  chaseTimer = 300;
}

#include "object.h"

float Vector::magnitude(){
  return sqrt(x*x + y*y);
}
bool Vector::normalize(){
  float mag = magnitude();
  if (mag > 0){
    x=x/mag;
    y=y/mag;
  }
}

Object::Object(ObjectType objType_):objType(objType_),bmp_handle(NULL),map(NULL),
    x(0),y(0),width(0),height(0){}
void Object::render(){
   if (bmp_handle == NULL){
     std::cerr << "Handle is null\n";
   }
   map->renderBitmap(bmp_handle, x, y,height);
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

Player::Player():Object(PLAYER),xLocked(false),killed(false),won(false),isCaught(false){}
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
   chaseTimer = 1000; //Chase for 10 s.
}
bool Player::isKilled(){
  return killed;
}
void Player::kill(){
   killed = true;
}
bool Player::hasWon(){
  return won;
}
void Player::win(){
  won = true;
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

Switch::Switch():Object(SWITCH),font(NULL),code(0),codeString("0"),updateLabel(false),counter(0){}
void Switch::render(){
   map->renderBitmap(bmp_handle, x, y,height);
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

Scanner::Scanner():Object(SCANNER),isTracking(false),player(NULL),chaseTimer(0),moveSpeed(4){}
bool Scanner::collidedWith(Object *other){
  Player *player = dynamic_cast<Player*>(other);
  if (player == NULL){
    return false;
  }
  //Detect kill collisions on impact
  if (x < (player->x + player->width) &&
    (x + width) > player->x        &&
    y < (player->y + player->height) &&
    (y + height) > player->y
    ){
      player->kill();
      return true;
   } else { //Start scanning when within a given distance
      Vector vect;
      vect.x = (float)(player->x - x);
      vect.y = (float)(player->y - y);
      float magnitude = vect.magnitude();
      if (magnitude < DETECT_DIST){
         trackPlayer(player);
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

Honeypot::Honeypot():Object(HONEYPOT){}
bool Honeypot::collidedWith(Object *other){
  Player *player = dynamic_cast<Player*>(other);
  if (player == NULL){
    return false;
  }
  //Detect kill collisions on impact
  if (x < (player->x + player->width) &&
     (x + width) > player->x        &&
     y < (player->y + player->height) &&
     (y + height) > player->y
     ){
     player->kill();
     return true;
  } else { //Start scanning when within a given distance
     Vector vect;
     vect.x = (float)(other->x - x);
     vect.y = (float)(other->y - y);
     float magnitude = vect.magnitude();
     if (magnitude < DETECT_DIST){
        player->honeypotCatch(this);
        return true;
    }
  }
  return false;
}


Firewall::Firewall():Object(FIREWALL),x1_disp(0),y1_disp(0),x2_disp(400),
    y2_disp(0),movingOut(true){}
void Firewall::render(){
  long x1 = x + x1_disp;
  long x2 = x + x2_disp;
  map->renderBitmap(bmp_handle,x1,y,height);
  map->renderBitmap(bmp_handle,x2,y,height);
}
bool Firewall::collidedWith(Object *other){
  long x1 = x + x1_disp;
  long x2 = x + x2_disp;
  Player *player = dynamic_cast<Player*>(other);
  if (player == NULL){
    return false;
  }
  //Detect kill collisions with first firewall rectangle:
  if (x1 < (player->x + player->width) &&
     (x1 + width) > player->x        &&
     y < (player->y + player->height) &&
     (y + height) > player->y
     ){
     player->kill();
     return true;
  } else if (x2 < (player->x + player->width) &&
     (x2 + width) > player->x        &&
     y < (player->y + player->height) &&
     (y + height) > player->y
     ){
     player->kill();
     return true;
  }
  return false;
}
void Firewall::animate(){
  if (movingOut){
    if (x1_disp > -100){
      x1_disp-=1;
      x2_disp+=1;
    }
    else {
      movingOut = false;
    }
  }
  else {
    if (x1_disp < 0){
      x1_disp+=1;
      x2_disp-=1;
    }
    else {
      movingOut = true;
    }
  }
}

Flag::Flag():Object(FLAG){}
bool Flag::collidedWith(Object *other){
  Player *player = dynamic_cast<Player*>(other);
  if (player == NULL){
    return false;
  }
  //Detect win condition on hitting the flag:
  if (x < (player->x + player->width) &&
     (x + width) > player->x        &&
     y < (player->y + player->height) &&
     (y + height) > player->y
     ){
     player->win();
     return true;
  }
  return false;
}

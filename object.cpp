#include "object.h"

Object::Object(){}
void Object::render(){
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



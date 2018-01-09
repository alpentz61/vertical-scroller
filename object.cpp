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

void Switch::render(){
   map->renderBitmap(bmp_handle, x, y);
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
      
      return true;
   }
   return false;
}
void Switch::animate(){

}

#include "object.h"

Object::Object(){}
Object::Object(Map *map_):map(map_){}
void Object::render(){
   al_draw_bitmap(bmp_handle, x, y, 0);
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
//
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

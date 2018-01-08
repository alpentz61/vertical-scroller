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

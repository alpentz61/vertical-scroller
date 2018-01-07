#include "object.h"

Object::Object(){
}

void Object::render(){
	al_draw_bitmap(bmp_handle, x, y, 0);
}

void Object::setBitmapHwnd(ALLEGRO_BITMAP *bmp_handle_){
	bmp_handle = bmp_handle_;
	width = al_get_bitmap_width(bmp_handle);
	height = al_get_bitmap_height(bmp_handle);
} 


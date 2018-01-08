#include "map.h"

Map::Map(){
   screenY = 0;
}
bool Map::initialize(){
   return false;
}
void Map::render(){
}
void Map::renderZone(int zoneIndex){
}
void Map::renderBitmap(ALLEGRO_BITMAP *bitmap, long x, long y){
   long renderY = ZONE_HEIGHT - (y - screenY);
   al_draw_bitmap(bitmap, x, renderY, 0);
}
bool Map::collidedWith(Object& obj){ 
   return false;
}	
bool Map::collidedWithZone(Object& obj, int zoneIndex){
   return false; 
}
int Map::getZoneIndex(long worldY){
   return -1;
}

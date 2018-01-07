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
void Map::renderObject(Object& obj){
   long renderY = ZONE_HEIGHT - (obj.y - screenY);
   al_draw_bitmap(obj.bmp_handle, obj.x, renderY, 0);
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

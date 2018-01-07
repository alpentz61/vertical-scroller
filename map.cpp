#include "map.h"

Map::Map(){

}
void Map::renderZone(int zoneIndex){
}
void Map::renderObject(Object& obj){
}
bool Map::collidedWith(Object& obj){
   return false;
}	
bool Map::collidedWithZone(Object& obj, int zoneIndexc){
   return false;
}
int Map::getZoneIndex(long worldY){
   return -1;
}

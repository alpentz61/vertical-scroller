#include "map.h"

Map::Map(){
   screenY = 0;
}
bool Map::initialize(){

   /*if (!loadConfig("map_cfg.json")){
     return false;
   }
   */
   return true;
}
bool Map::loadResources(){
   bool success = false;
   //Load the font
   font = al_load_ttf_font("chintzys.ttf",72,0 );
   if (!font){
      fprintf(stderr, "Could not load 'chintzys.ttf'.\n");
      goto FAILED_FONT;
   }

   //Load the honeypot sprites:
   honeypot_bitmap = al_load_bitmap("images/pumpkin_open_clipart.png");
   if (!honeypot_bitmap){
      fprintf(stderr, "failed to create honeypot bitmap!\n");
      goto FAILED_HONEYPOT;
   }

   //Load the network switch sprite:
   switch_bitmap = al_load_bitmap("images/network_switch.jpg");
   if (!switch_bitmap){
      fprintf(stderr, "failed to create network switch bitmap!\n");
      goto FAILED_SWITCH;
   }

   //Load the firewall bitmap
   firewall_bitmap = al_load_bitmap("images/firewall_open_clipart.png");
   if (!firewall_bitmap){
      fprintf(stderr, "failed to create firewall bitmap!\n");
      goto FAILED_FIREWALL;
   }

   //Load scanner bitmap
   scanner_bitmap = al_load_bitmap("images/scanner_open_clipart.png");
   if (!scanner_bitmap){
      fprintf(stderr, "failed to create scanner bitmap!\n");
      goto FAILED_SCANNER;
   }

   success = true;

   al_destroy_bitmap(scanner_bitmap);
FAILED_SCANNER:
   al_destroy_bitmap(firewall_bitmap);
FAILED_FIREWALL:
   al_destroy_bitmap(switch_bitmap);
FAILED_SWITCH:
   al_destroy_bitmap(honeypot_bitmap);
FAILED_HONEYPOT:
   al_destroy_font(font);
FAILED_FONT:
   return success;
}

/*
bool Map::loadConfig(const std::string& cfg_file) {
   std::ifstream infile(cfg_file);
   Json::Value root;
   Json::Reader reader;
   bool b = reader.parse(infile,root);
   if (!b){
     std::cerr << "Error parsing the map config file.\n";
     return false;
   }
   for (Json::Value::iterator it = root.begin(); it != root.end(); it++){
      Json::Value key = it.key();
      Json::Value value = (*it);
      std::cout<<"Key: "<<key.toStyledString();
      std::cout<<"Value: "<<value.toStyledString();
   }
}
*/
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

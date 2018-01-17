#include "map.h"

Map::Map(){
   screenY = 0;
}
bool Map::initialize(){
   //load resources
   if (!loadResources()){
     return false;
   }

   //Create game objects
   //honeypot:
   const int NUM_POTS = 16;
   Honeypot pot_objects[NUM_POTS];
   for (int i=0; i<NUM_POTS; i++){
      pot_objects[i].setBitmapHwnd(honeypot_bitmap);
      pot_objects[i].map = this;
   }
   pot_objects[0].x = 0;
   pot_objects[0].y = 100;
   pot_objects[1].x = 700;
   pot_objects[1].y = 100;
   pot_objects[2].x = 0;
   pot_objects[2].y = 800;
   pot_objects[3].x = 700;
   pot_objects[3].y = 800;
   pot_objects[4].x = 0;
   pot_objects[4].y = 1000;
   pot_objects[5].x = 200;
   pot_objects[5].y = 1200;
   pot_objects[6].x = 400;
   pot_objects[6].y = 1400;
   pot_objects[7].x = 600;
   pot_objects[7].y = 1900;
   pot_objects[8].x = 0;
   pot_objects[8].y = 1900;
   pot_objects[9].x = 100;
   pot_objects[9].y = 1900;
   pot_objects[10].x = 200;
   pot_objects[10].y = 1900;
   pot_objects[11].x = 300;
   pot_objects[11].y = 2200;
   pot_objects[12].x = 400;
   pot_objects[12].y = 2200;
   pot_objects[13].x = 500;
   pot_objects[13].y = 2200;
   pot_objects[14].x = 100;
   pot_objects[14].y = 2500;
   pot_objects[15].x = 600;
   pot_objects[15].y = 2500;
   for (int i=0; i<NUM_POTS; i++){
      Honeypot *honeypot = new Honeypot;
      *honeypot = pot_objects[i];
      objs.push_back(honeypot);
   }

   //network switch
   Switch *nwSwitch = new Switch;
   nwSwitch->setBitmapHwnd(switch_bitmap);
   nwSwitch->map = this;
   nwSwitch->x = 0;
   nwSwitch->y = 1600;
   nwSwitch->font = font;
   objs.push_back(nwSwitch);

   Scanner *scanner = new Scanner;
   scanner->setBitmapHwnd(scanner_bitmap);
   scanner->map = this;
   scanner->x = 0;
   scanner->y= 1000;
   objs.push_back(scanner);

   Firewall *firewall = new Firewall;
   firewall->setBitmapHwnd(firewall_bitmap);
   firewall->map = this;
   firewall->x = 0;
   firewall->y = 800;
   objs.push_back(firewall);

   //Paritition the objects into zones
   for (std::list<Object*>::iterator it = objs.begin(); it != objs.end(); it++){
     int zoneIndex = getZoneIndex((*it)->y);
     std::cout << "Adding object to zone " << zoneIndex << "\n";
     if (zoneIndex < NUM_ZONES){
       objs_zoned.at(zoneIndex).push_back((*it));
     }
     else {
       std::cerr << "Error - zone index is out of range.\n";
       return false;
     }
   }
   return true;
}
bool Map::loadResources(){
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

   return true; //TODO: Handle cleanup of bitmaps after they are done being used.

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
   return false;
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
  int zoneIndex = getZoneIndex(screenY);
  if (zoneIndex > 0){
    renderZone(zoneIndex-1);
  }
  renderZone(zoneIndex);
  if (zoneIndex < (NUM_ZONES-1)){
    renderZone(zoneIndex+1);
  }
}
void Map::renderZone(int z){
   for (std::list<Object*>::iterator it = objs_zoned[z].begin();
         it != objs_zoned[z].end(); it++){
      long renderY = (*it)->y - screenY;
      if (renderY > (*it)->height || renderY < ZONE_HEIGHT) {
        (*it)->render();
      }
   }
}
void Map::renderBitmap(ALLEGRO_BITMAP *bitmap, long x, long y,long height){
   long renderY = ZONE_HEIGHT - ((y - screenY)+height);
   al_draw_bitmap(bitmap, x, renderY, 0);
}
void Map::animate(){
  int zoneIndex = getZoneIndex(screenY);
  if (zoneIndex > 0){
    animateZone(zoneIndex-1);
  }
  animateZone(zoneIndex);
  if (zoneIndex < (NUM_ZONES-1)){
    animateZone(zoneIndex+1);
  }
}
void Map::animateZone(int z){
  for (std::list<Object*>::iterator it = objs_zoned[z].begin();
       it != objs_zoned[z].end(); it++){
    (*it)->animate();
  }
}
bool Map::collidedWith(Object& obj){
  bool collided = false;
  int zoneIndex = getZoneIndex(screenY);
  if (zoneIndex > 0){
    if(collidedWithZone(obj,zoneIndex-1))collided = true;
  }
  if(collidedWithZone(obj,zoneIndex))collided = true;
  if (zoneIndex < (NUM_ZONES-1)){
    if(collidedWithZone(obj,zoneIndex+1))collided = true;
  }
  return collided;
}
bool Map::collidedWithZone(Object& obj, int z){
   bool collided = false;
   for (std::list<Object*>::iterator it = objs_zoned[z].begin();
         it != objs_zoned[z].end(); it++){
      if((*it)->collidedWith(&obj)){
        collided = true;
        printf("collided\n");
      }
   }
   return collided;
}
int Map::getZoneIndex(long worldY){
   return worldY / ZONE_HEIGHT;
}

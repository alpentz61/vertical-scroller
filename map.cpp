#include "map.h"

Map::Map(){
   screenY = 0;
}
bool Map::initialize(){
   //load resources
   if (!loadResources()){
     return false;
   }

   std::string cfgFile = "map_cfg.json";
   if (!loadConfig(cfgFile)){
     return false;
   }

   for (std::list<ObjectConfig>::iterator it = config.begin(); it != config.end();it++){
     ObjectConfig cfg = (*it);
     if (cfg.type.compare("Switch")==0){
       //network switch
       Switch *nwSwitch = new Switch;
       nwSwitch->setBitmapHwnd(switch_bitmap);
       nwSwitch->map = this;
       nwSwitch->x = cfg.x;
       nwSwitch->y = cfg.y;
       nwSwitch->font = font;
       objs.push_back(nwSwitch);
     }
     else if (cfg.type.compare("Scanner")==0){
       Scanner *scanner = new Scanner;
       scanner->setBitmapHwnd(scanner_bitmap);
       scanner->map = this;
       scanner->x = cfg.x;
       scanner->y= cfg.y;
       objs.push_back(scanner);
     }
     else if (cfg.type.compare("Honeypot")==0){
       Honeypot *honeypot = new Honeypot;
       honeypot->setBitmapHwnd(honeypot_bitmap);
       honeypot->map = this;
       honeypot->x = cfg.x;
       honeypot->y = cfg.y;
       objs.push_back(honeypot);
     }
     else if (cfg.type.compare("Firewall")==0){
       Firewall *firewall = new Firewall;
       firewall->setBitmapHwnd(firewall_bitmap);
       firewall->map = this;
       firewall->x = cfg.x;
       firewall->y = cfg.y;
       objs.push_back(firewall);
     }
     else if (cfg.type.compare("Flag")==0){
       Flag *flag = new Flag;
       flag->setBitmapHwnd(flag_bitmap);
       flag->map = this;
       flag->x = cfg.x;
       flag->y = cfg.y;
       objs.push_back(flag);
     }
   }

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
   honeypot_bitmap = al_load_bitmap("images/honeypot_open_clipart.png");
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

   //Load flag bitmap
   flag_bitmap = al_load_bitmap("images/flag_open_clipart.png");
   if (!flag_bitmap){
      fprintf(stderr, "failed to create flag bitmap!\n");
      goto FAILED_FLAG;
   }

   return true; //TODO: Handle cleanup of bitmaps after they are done being used if no error.

   al_destroy_bitmap(flag_bitmap);
FAILED_FLAG:
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
      Json::Value obj = (*it);
      ObjectConfig cfg;
      cfg.x = obj["x"].asInt();
      cfg.y = obj["y"].asInt();
      cfg.type = obj["type"].asString();
      config.push_back(cfg);
   }
   return true;
}

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
long Map::getScreenY(long worldY,long height){
  return ZONE_HEIGHT - ((worldY - screenY)+height);
}

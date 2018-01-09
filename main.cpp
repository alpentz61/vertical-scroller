
#include "game.h"
#include "object.h"
#include "map.h"

//To Build: g++ main.cpp -lallegro -lallegro_image

const float FPS = 60;
const int SCREEN_W = 800;
const int SCREEN_H = 800;
enum MYKEYS {
   KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};

const char* PLAYER_FILE = "images/F35_Sketched_V2_Scaled.png";
const char* HONEY_POT_FILE = "images/pumpkin_open_clipart.png";

int main(int argc, char **argv)
{
   bool success = false; 
   ALLEGRO_DISPLAY *display = NULL;
   ALLEGRO_EVENT_QUEUE *event_queue = NULL;
   ALLEGRO_TIMER *timer = NULL;
   bool key[4] = { false, false, false, false };
   bool redraw = true;
   bool doexit = false;
   //Player resource
   ALLEGRO_BITMAP *player = NULL;
   const int PLAYER_SIZE = 32;
   long player_x = SCREEN_W / 2.0 - PLAYER_SIZE / 2.0;
   long player_y = SCREEN_H / 2.0 - PLAYER_SIZE / 2.0;
   //Honeypot
   ALLEGRO_BITMAP *honeypot = NULL;
   const int NUM_TEST_POTS = 8;	   
   Object pot_objects[NUM_TEST_POTS];
   //Network Switch
   ALLEGRO_BITMAP *switch_bitmap = NULL;
   Switch nwSwitch;
   //Map
   Map map;  
   //Font
   ALLEGRO_FONT *font = NULL;

   //Initialize allegro
   if(!al_init()) {
      fprintf(stderr, "failed to initialize allegro!\n");
      goto FAILED;
   }

   //Initialize the image addon portion of the library
   if(!al_init_image_addon()) {
      fprintf(stderr, "failed to initialize the image addon!\n");
      goto FAILED;
   }

   //Initialize the font add on
   if (!al_init_font_addon()){
      fprintf(stderr, "failed to initialize the font addon!\n");
      goto FAILED;
   }

   //Initialize the ttf add on
   if (!al_init_ttf_addon()){
      fprintf(stderr, "failed to initialize ttf addon!\n");
      goto FAILED;
   }

   //Load the font
   font = al_load_ttf_font("chintzys.ttf",72,0 );
   if (!font){
      fprintf(stderr, "Could not load 'chintzys.ttf'.\n");
      goto FAILED;
   }
 
   //Initialize the keybaord handler
   if(!al_install_keyboard()) {
      fprintf(stderr, "failed to initialize the keyboard!\n");
      goto FAILED;
   }

   //Initialize the timer
   timer = al_create_timer(1.0 / FPS);
   if(!timer) {
      fprintf(stderr, "failed to create timer!\n");
      goto FAILED;
   }
 
   //Initialize the display
   display = al_create_display(SCREEN_W, SCREEN_H);
   if(!display) {
      fprintf(stderr, "failed to create display!\n");
      goto FAILED_DISPLAY;
   }
   al_clear_to_color(al_map_rgb(0,0,0));
   al_flip_display();

   //Load image resources --- Note: This must be performed after initializing the displa
   //Load the player sprite:
   player = al_load_bitmap(PLAYER_FILE);
   if (!player){
      fprintf(stderr, "failed to create player bitmap!\n");
      goto FAILED_PLAYER;
   }

   //Load the honeypot sprites:
   honeypot = al_load_bitmap(HONEY_POT_FILE);
   if (!honeypot){
      fprintf(stderr, "failed to create honeypot bitmap!\n");
      goto FAILED_HONEYPOT;
   }

   //Load the network switch sprite:
   switch_bitmap = al_load_bitmap("images/network_switch.jpg");
   if (!switch_bitmap){
      fprintf(stderr, "failed to create network switch bitmap!\n");
      goto FAILED_HONEYPOT; //TODO: Proper memory management
   }

   //Initialize the event queue
   event_queue = al_create_event_queue();
   if(!event_queue) {
      fprintf(stderr, "failed to create event_queue!\n");
      goto FAILED_EVENT;
   }
   al_register_event_source(event_queue, al_get_display_event_source(display));
   al_register_event_source(event_queue, al_get_timer_event_source(timer));
   al_register_event_source(event_queue, al_get_keyboard_event_source());

   //Initilize the test objects:
   for (int i=0; i<NUM_TEST_POTS; i++){
      pot_objects[i].setBitmapHwnd(honeypot);
      pot_objects[i].map = &map;
   }
   pot_objects[0].x = 0;
   pot_objects[0].y = 100;
   pot_objects[1].x = 800; 
   pot_objects[1].y = 100;
   pot_objects[2].x = 0;
   pot_objects[2].y = 800;
   pot_objects[3].x = 800;
   pot_objects[3].y = 800;
   pot_objects[4].x = 0;
   pot_objects[4].y = 1000;
   pot_objects[5].x = 200;
   pot_objects[5].y = 1200;
   pot_objects[6].x = 400;
   pot_objects[6].y = 1400;
   pot_objects[7].x = 600;
   pot_objects[7].y = 1600;
   //Network switch test object
   nwSwitch.setBitmapHwnd(switch_bitmap);
   nwSwitch.map = &map;
   nwSwitch.x = 0;
   nwSwitch.y = 1600;
   nwSwitch.font = font;
  

   al_start_timer(timer);
   while(!doexit)
   {
      ALLEGRO_EVENT ev;
      al_wait_for_event(event_queue, &ev);
 
      if(ev.type == ALLEGRO_EVENT_TIMER) {
         if(key[KEY_UP] && (player_y-map.screenY) >= 4.0) {
            player_y -= 4.0;
         }

         if(key[KEY_DOWN] && (player_y-map.screenY) <= SCREEN_H - PLAYER_SIZE - 4.0) {
            player_y += 4.0;
         }

         if(key[KEY_LEFT] && player_x >= 4.0) {
            player_x -= 4.0;
         }

         if(key[KEY_RIGHT] && player_x <= SCREEN_W - PLAYER_SIZE - 4.0) {
            player_x += 4.0;
         }
         map.screenY += 1;
         redraw = true;
      }
      else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
         break;
      }
      else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
         switch(ev.keyboard.keycode) {
            case ALLEGRO_KEY_UP:
               key[KEY_UP] = true;
               break;

            case ALLEGRO_KEY_DOWN:
               key[KEY_DOWN] = true;
               break;

            case ALLEGRO_KEY_LEFT: 
               key[KEY_LEFT] = true;
               break;

            case ALLEGRO_KEY_RIGHT:
               key[KEY_RIGHT] = true;
               break;
         }
      }
      else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
         switch(ev.keyboard.keycode) {
            case ALLEGRO_KEY_UP:
               key[KEY_UP] = false;
               break;

            case ALLEGRO_KEY_DOWN:
               key[KEY_DOWN] = false;
               break;

            case ALLEGRO_KEY_LEFT: 
               key[KEY_LEFT] = false;
               break;

            case ALLEGRO_KEY_RIGHT:
               key[KEY_RIGHT] = false;
               break;

            case ALLEGRO_KEY_ESCAPE:
               doexit = true;
               break;
         }
      }
 
      if(redraw && al_is_event_queue_empty(event_queue)) {
         redraw = false;
 
         al_clear_to_color(al_map_rgb(0,0,0));

         //Render Text:
          al_draw_text(font, al_map_rgb(255,255,255), 0,0,0, "Your Text Here!");

	 //Render Player
 	 Object obj;
	 obj.setBitmapHwnd(player);
         obj.map = &map;
	 obj.x = player_x;
	 obj.y = player_y;
	 obj.render();

	 //Render honeypot test objects
	 for (int i=0; i<NUM_TEST_POTS; i++){
            pot_objects[i].render();
         }
  	  
         //Test collision with honeypots
         for (int i=0; i<NUM_TEST_POTS; i++){
            pot_objects[i].collidedWith(&obj);
         }

         //Test collisions with the nwSwitch:
         nwSwitch.collidedWith(&obj);
         nwSwitch.animate();

         if (obj.collList.size()>0){
            for (std::list<Collision>::const_iterator it = obj.collList.begin();
               it != obj.collList.end(); it++){
               printf("Coll Type %d\n",it->collType);
            }
         }

         
         //Render the switch:
         nwSwitch.render();

         al_flip_display();
      }
   }
   success = true;

   al_destroy_event_queue(event_queue);
FAILED_EVENT:
   al_destroy_bitmap(honeypot);
FAILED_HONEYPOT:
   al_destroy_bitmap(player);
FAILED_PLAYER:
   al_destroy_display(display);
FAILED_DISPLAY:
   al_destroy_timer(timer);
FAILED:
   if (success){
      return 0;
   }
   else {
      return -1;
   }
}


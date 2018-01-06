#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "object.h"

//To Build: g++ main.cpp -lallegro -lallegro_image

const float FPS = 60;
const int SCREEN_W = 1280;
const int SCREEN_H = 800;
const char* PLAYER_FILE = "images/F35_Sketched_V2_Scaled.png";
const char* HONEY_POT_FILE = "images";
enum MYKEYS {
   KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};

int main(int argc, char **argv)
{
   ALLEGRO_DISPLAY *display = NULL;
   ALLEGRO_EVENT_QUEUE *event_queue = NULL;
   ALLEGRO_TIMER *timer = NULL;
   bool key[4] = { false, false, false, false };
   bool redraw = true;
   bool doexit = false;

   Object obj;

   //Initialize allegro
   if(!al_init()) {
      fprintf(stderr, "failed to initialize allegro!\n");
      return -1;
   }

   //Initialize the image addon portion of the library
   if(!al_init_image_addon()) {
      fprintf(stderr, "failed to initialize the image addon!\n");
      return -1;
   }
 
   //Initialize the keybaord handler
   if(!al_install_keyboard()) {
      fprintf(stderr, "failed to initialize the keyboard!\n");
      return -1;
   }

   //Initialize the timer
   timer = al_create_timer(1.0 / FPS);
   if(!timer) {
      fprintf(stderr, "failed to create timer!\n");
      return -1;
   }
 
   //Initialize the display
   display = al_create_display(SCREEN_W, SCREEN_H);
   if(!display) {
      fprintf(stderr, "failed to create display!\n");
      //TODO: Move all destroys to a goto statement at the end
      al_destroy_timer(timer);
      return -1;
   }
   al_clear_to_color(al_map_rgb(0,0,0));
   al_flip_display();

 
   //Load image resources --- Note: This must be performed after initializing the display
   //Load the player sprite:
   ALLEGRO_BITMAP *player = NULL;
   const int PLAYER_SIZE = 32;
   float player_x = SCREEN_W / 2.0 - PLAYER_SIZE / 2.0;
   float player_y = SCREEN_H / 2.0 - PLAYER_SIZE / 2.0;
   player = al_load_bitmap(PLAYER_FILE);
   if (!player){
      fprintf(stderr, "failed to create player bitmap!\n");
      al_destroy_display(display);
      al_destroy_timer(timer);
      return -1;
   }

   //Initialize the event queue
   event_queue = al_create_event_queue();
   if(!event_queue) {
      fprintf(stderr, "failed to create event_queue!\n");
      al_destroy_bitmap(player);
      al_destroy_display(display);
      al_destroy_timer(timer);
      return -1;
   }
   al_register_event_source(event_queue, al_get_display_event_source(display));
   al_register_event_source(event_queue, al_get_timer_event_source(timer));
   al_register_event_source(event_queue, al_get_keyboard_event_source());

   al_start_timer(timer);
   while(!doexit)
   {
      ALLEGRO_EVENT ev;
      al_wait_for_event(event_queue, &ev);
 
      if(ev.type == ALLEGRO_EVENT_TIMER) {
         if(key[KEY_UP] && player_y >= 4.0) {
            player_y -= 4.0;
         }

         if(key[KEY_DOWN] && player_y <= SCREEN_H - PLAYER_SIZE - 4.0) {
            player_y += 4.0;
         }

         if(key[KEY_LEFT] && player_x >= 4.0) {
            player_x -= 4.0;
         }

         if(key[KEY_RIGHT] && player_x <= SCREEN_W - PLAYER_SIZE - 4.0) {
            player_x += 4.0;
         }

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

         al_draw_bitmap(player, player_x, player_y, 0);
 
         al_flip_display();
      }
   }
 
   al_destroy_bitmap(player);
   al_destroy_timer(timer);
   al_destroy_display(display);
   al_destroy_event_queue(event_queue);
 
   return 0;
}



#include "game.h"
#include "object.h"
#include "map.h"

//To Build: g++ main.cpp -lallegro -lallegro_image

const float FPS = 60;
//const int SCREEN_W = 800;
const int SCREEN_H = 800;
enum MYKEYS {
   KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};

const char* PLAYER_FILE = "images/F35_Sketched_V2_Scaled.png";
const char* HONEY_POT_FILE = "images/pumpkin_open_clipart.png";

int main(int argc, char **argv)
{
   bool success = false;
   bool gameOver = false;
   bool playerWins = false;
   int gameOverTimer = 500;
   ALLEGRO_DISPLAY *display = NULL;
   ALLEGRO_EVENT_QUEUE *event_queue = NULL;
   ALLEGRO_TIMER *timer = NULL;
   bool key[4] = { false, false, false, false };
   bool redraw = true;
   bool doexit = false;
   //Player resource
   Player player_obj;
   ALLEGRO_BITMAP *player = NULL;
   const int PLAYER_SIZE = 32;

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
      fprintf(stderr, "Could not load 'chintzys_.ttf'.\n");
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

   //Load the player sprite:
   player = al_load_bitmap(PLAYER_FILE);
   if (!player){
      fprintf(stderr, "failed to create player bitmap!\n");
      goto FAILED_PLAYER;
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

   if(!map.initialize()){
     goto FAILED; //TODO: Proper memory management
   }

   player_obj.setBitmapHwnd(player);
   player_obj.map = &map;
   player_obj.x = SCREEN_W / 2.0 - PLAYER_SIZE / 2.0;
   player_obj.y = SCREEN_H / 2.0 - PLAYER_SIZE / 2.0;

   al_start_timer(timer);
   while(!doexit)
   {
      ALLEGRO_EVENT ev;
      al_wait_for_event(event_queue, &ev);

      if(ev.type == ALLEGRO_EVENT_TIMER) {

         if(key[KEY_UP]){
           player_obj.moveUp();
         }

         if (key[KEY_DOWN]){
           player_obj.moveDown();
         }

         long screenY = map.getScreenY(player_obj.y,player_obj.height);
         if ((screenY < 0) || (screenY > SCREEN_H-player_obj.height)){
           player_obj.kill();
         }

         if(key[KEY_LEFT] && player_obj.x >= 4.0) {
            player_obj.moveLeft();
         }

         if(key[KEY_RIGHT] && player_obj.x <= SCREEN_W - PLAYER_SIZE - 4.0) {
            player_obj.moveRight();
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

         bool xLocked = false;
         //Handle collisions:
         map.collidedWith(player_obj);
         for (std::list<Collision>::iterator it = player_obj.collList.begin();
               it != player_obj.collList.end(); it++){
            Collision *collision = &(*it);
            //1. Switch collisions
            if (collision->collObject->objType == SWITCH){
              Switch *nwSwitch = dynamic_cast<Switch*>(collision->collObject);
              if (nwSwitch != NULL){
                if (collision->collType == SWITCH_PASS){
                  //xLocked creates a special case requiring collision handling here
                  xLocked = true;
                }
                else if (collision->collType == SWITCH_KILL){
                  player_obj.kill();
                }
              }
            }
            //2. Scanner Collisions... Handled in collidedWith()
            //3. Firewall Collisions... Handled in collidedWith()
            //4. Honepot Collisiions... Handled in collidedWith()
         }
         player_obj.collList.clear();

         //Lock player x if in network switch tunnel:
         if (xLocked){
           player_obj.setXLocked(true);
         }
         else {
           player_obj.setXLocked(false);
         }

         //Animate the game objects
         map.animate();

         //Animate the player
         player_obj.animate();

         //Render player while game is on going
         if (!gameOver){
           player_obj.render();
         }

         //End the game if the player is killed or wins
         if (player_obj.isKilled() || player_obj.hasWon()){
           playerWins = player_obj.hasWon();
           gameOver = true;
         }

         //Render all game objects currently active
         map.render();

         //Display game over message then exit the game
         if (gameOver){
           char *winMsg;
           if (playerWins){
             winMsg = "You Win!";
           }
           else {
             winMsg = "You Lose!";
           }
           al_draw_text(font, al_map_rgb(255,255,255), 230,300,0, "Game Over");
           al_draw_text(font, al_map_rgb(255,255,255), 250,400,0, winMsg);
           gameOverTimer--;
           if (gameOverTimer <= 0){
             doexit = true;
           }
         }

         al_flip_display();
      }
   }
   success = true;

   al_destroy_event_queue(event_queue);
FAILED_EVENT:
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

#include "utilities.h"
#include "ship.h"
int between(int low, int high){
    return low + (rand() % (high - low));
}


float between_f (float low, float high){
    return low + ((float)rand() / (float)RAND_MAX) / (high - low);
}

bool collide(int obj1_x, int obj1_y, int obj1_w, int obj1_h,
               int obj2_x, int obj2_y, int obj2_w, int obj2_h){
    if ((obj1_x + obj1_w < obj2_x) || (obj1_x > obj2_x + obj2_w) ||
        (obj1_y + obj1_h < obj2_y) || (obj1_y > obj2_y + obj2_h))
        return false;
    return true;
}

void must_init(bool test, const char* description){
    if (test) return;

    std::cout << "can not initialize " << description << std::endl;
    exit(1);
}

//extract sprite from _sheet.
ALLEGRO_BITMAP* sprite_grab(ALLEGRO_BITMAP* bigmap,int x, int y, int w, int h){
    ALLEGRO_BITMAP* sprite = al_create_sub_bitmap(bigmap, x, y, w, h);
    must_init(sprite, "sprite grab");
    return sprite;
}

void drawPlayerInformation(ALLEGRO_FONT* font,Score* score, Ship* ship1, Ship* ship2){
    al_draw_textf(font, al_map_rgb_f(1,1,1), 3, 3, 0,
                  "%ld", score->getScore());
    if (!(ship1->getLives() || ship2->getLives()))
        al_draw_text(font, al_map_rgb_f(1,1,1),DISPLAY_W/2, DISPLAY_H/2,
                     ALLEGRO_ALIGN_CENTER, "G A M E  O V E R");
}
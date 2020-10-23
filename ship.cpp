#include "ship.h"

extern ALLEGRO_CONFIG* config;
extern int PRIM_DISPLAY_W, PRIM_DISPLAY_H;
extern int EFFECTIVE_DISPLAY_DIAG;
extern int EFFECTIVE_DRAWING_DIMENSION;
extern float FRAMERATEMULTIPLIER;

Ship::Ship (ALLEGRO_BITMAP* bitmap, int x, int y){
    ship_img = bitmap;
    pos_x = x;
    pos_y = y;
    width = al_get_bitmap_width(bitmap);
    height = al_get_bitmap_height(bitmap);
    fireWait = 8*FRAMERATEMULTIPLIER;
    type = SHIP;
    must_init(ship_img, "ship initialization");
    speed = EFFECTIVE_DISPLAY_DIAG/140;
    respawnTimer = 60*FRAMERATEMULTIPLIER;
    blink_respawn_counter = respawnTimer;
}

void Ship::draw(){
    if (!life)
        return; //need add more stuff
    if (!gotHit)
    {
        al_draw_bitmap(ship_img, pos_x, pos_y, 0);
        return;
    }
    //got hit
    if ((int)(blink_respawn_counter/(2*FRAMERATEMULTIPLIER)) % 2)
    {
        al_draw_bitmap(ship_img, pos_x, pos_y, 0);
    }
}

void Ship::update (Keyboard& keyboard){
    if (!life)
        return;
    if (respawning)
    {
        if (!(--blink_respawn_counter))
        {
            respawning = false;
            gotHit = false;
            blink_respawn_counter = respawnTimer;
        }
        return;
    }
    if (fireCountdown > 0)
        --fireCountdown;
    else fireCountdown = 0;
    if (default_control)
        update_util(keyboard, ALLEGRO_KEY_UP, ALLEGRO_KEY_DOWN,
                    ALLEGRO_KEY_LEFT, ALLEGRO_KEY_RIGHT, ALLEGRO_KEY_M);
    else update_util(keyboard, new_up, new_down, new_left, new_right,
                     new_shoot);
}

void Ship::set_control(int up, int down, int left, int right, int shoot){
    default_control = false;
    new_up = up;
    new_down = down;
    new_left = left;
    new_right = right;
    new_shoot = shoot;
}

void Ship::update_util(Keyboard& keyboard, int up, int down,
                 int left, int right, int shoot){
    if (keyboard.key[up])
        pos_y -= EFFECTIVE_DISPLAY_DIAG/140;
    if (keyboard.key[down])
        pos_y += EFFECTIVE_DISPLAY_DIAG/140;
    if (keyboard.key[left])
        pos_x -= EFFECTIVE_DISPLAY_DIAG/140;
    if (keyboard.key[right])
        pos_x += EFFECTIVE_DISPLAY_DIAG/140;
    if (pos_x < 0)
        pos_x = 0;
    if (pos_x > PRIM_DISPLAY_W  - width)
        pos_x = PRIM_DISPLAY_W  - width;
    if (pos_y < 0)
        pos_y = 0;
    if (pos_y > PRIM_DISPLAY_H  - height)
        pos_y = PRIM_DISPLAY_H  - height;
    if (keyboard.key[shoot])
        shoot_command();
}

Ship_factory::Ship_factory(ALLEGRO_BITMAP* sheet){
    int x = std::atoi(al_get_config_value(config, "components", "ship_x"));
    int y = std::atoi(al_get_config_value(config, "components", "ship_y"));
    int w = std::atoi(al_get_config_value(config, "components", "ship_w"));
    int h = std::atoi(al_get_config_value(config, "components", "ship_h"));
    ALLEGRO_BITMAP* tempBmp = sprite_grab(sheet, x, y, w, h);
    float bmpRatio = (float) w / (float) h;
    ALLEGRO_BITMAP* saveDisplay = al_get_target_bitmap();
    shipImg = al_create_bitmap(bmpRatio * EFFECTIVE_DRAWING_DIMENSION/20,
                               EFFECTIVE_DRAWING_DIMENSION/20);
    al_set_target_bitmap(shipImg);
    al_clear_to_color(al_map_rgba_f(0,0,0,0));
    al_draw_scaled_bitmap(tempBmp,0,0,w,h,0,0,al_get_bitmap_width(shipImg),
                          al_get_bitmap_height(shipImg),0);
    al_set_target_bitmap(saveDisplay);
    al_destroy_bitmap(tempBmp);
}



#include "ship.h"

void Ship::draw(){
    if (!life)
        return; //need add more stuff
    if (!gotHit)
    {
        al_draw_bitmap(ship_img, pos_x, pos_y, 0);
        return;
    }
    //got hit
    ++blink_counter;
    if (!(blink_counter % 3))
    {
        al_draw_bitmap(ship_img, pos_x, pos_y, 0);
    }
    if (blink_counter == blinking)
    {
        blink_counter = 1;
        gotHit = false;
    }
}

void Ship::update (Keyboard* keyboard){
    if (!life)
        return;
    if (respawning)
    {
        if (--respawnTimer == 0)
        {
            respawning = false;
            respawnTimer = 60;
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

void Ship::update_util(Keyboard* keyboard, int up, int down,
                 int left, int right, int shoot){
    if (keyboard->key[up])
        pos_y -= speed;
    if (keyboard->key[down])
        pos_y += speed;
    if (keyboard->key[left])
        pos_x -= speed;
    if (keyboard->key[right])
        pos_x += speed;
    if (pos_x < 0)
        pos_x = 0;
    if (pos_x > DISPLAY_W  - width)
        pos_x = DISPLAY_W  - width;
    if (pos_y < 0)
        pos_y = 0;
    if (pos_y > DISPLAY_H  - height)
        pos_y = DISPLAY_H  - height;
    if (keyboard->key[shoot])
        shoot_command();
}

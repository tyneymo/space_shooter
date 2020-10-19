#ifndef SHIP_H
#define SHIP_H
#include <iostream>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include "utilities.h"
#include "bullet.h"

class Ship_factory;
bool bulletObjCollide(ShootableObject* obj, Bullet* bullet);
class Ship : public ShootableObject{
    friend class Ship_factory;

public:
    enum control {UP, DOWN, LEFT, RIGHT};

    ~Ship(){
        al_destroy_bitmap(ship_img);
        //destroy audio samples
    }

    void draw(){
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

    void update (Keyboard* keyboard){
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


    void set_control(int up, int down, int left, int right, int shoot){
        default_control = false;
        new_up = up;
        new_down = down;
        new_left = left;
        new_right = right;
        new_shoot = shoot;
    }

    void gotShoot(){
        gotHit = true;
        --life;
        respawning = true;
        //play a sound of explosion when get shoot
        al_play_sample(sample_explode[1],0.8,0,1.4, ALLEGRO_PLAYMODE_ONCE,NULL);
    }

    int getLives(){
        return life;
    }

    bool ifRespawning(){
        return respawning;
    }

private:
    Ship(ALLEGRO_BITMAP* bitmap, int x, int y, int w, int h): ship_img(bitmap) {
        pos_x = x;
        pos_y = y;
        width = w;
        height = h;
        fireWait = 5;
        type = SHIP;
    }

    void update_util(Keyboard* keyboard, int up, int down,
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
        if (pos_x > DISPLAY_W * SCALE)
            pos_x = DISPLAY_W * SCALE;
        if (pos_y < 0)
            pos_y = 0;
        if (pos_y > DISPLAY_H * SCALE)
            pos_y = DISPLAY_H * SCALE;
        if (keyboard->key[shoot])
            shoot_command();
    }

    void shoot_command(){
        if (fireCountdown <= 0)
        {
            fireNow = true;
            shootSound();
        }
    }

    void shootSound(){
        al_play_sample(shot_sample,0.8,0,1.1, ALLEGRO_PLAYMODE_ONCE,NULL);
    }


    ALLEGRO_BITMAP* ship_img;

    int speed = 3;
    bool default_control = true;
    int new_up, new_down, new_left, new_right, new_shoot;
    int life = 5;
    int blinking = 60;
    int blink_counter = 0;
    bool respawning = false;
    int respawnTimer = 60; //2 seconds
};

class Ship_factory{
public:
    Ship_factory(ALLEGRO_BITMAP* sheet): sprite(sheet){}
    Ship* createShip(int x, int y)    {
        ALLEGRO_BITMAP* ship_image = sprite_grab(sprite, shipImage_x,
                                                          shipImage_y,
                                                          shipImage_w,
                                                          shipImage_h
                                                          );
        return new Ship(ship_image, x, y, shipImage_w, shipImage_h);
    }

private:
    //sprite keep copy of a common sprite and should be destroyed by user
    ALLEGRO_BITMAP* sprite;
    int shipImage_x = 0, shipImage_y = 0, shipImage_w = 12, shipImage_h = 13;
};

#endif // SHIP_H

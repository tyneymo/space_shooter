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
        //destroy audio samples
    }

    void draw();
    void update (Keyboard* keyboard);
    void set_control(int up, int down, int left, int right, int shoot);

    void gotShoot(){
        gotHit = true;
        --life;
        respawning = true;
        //play a sound of explosion when get shot
        al_play_sample(sample_explode[1],0.8,0,1.4, ALLEGRO_PLAYMODE_ONCE,NULL);
    }

    int getLives(){
        return life;
    }

    bool ifRespawning(){
        return respawning;
    }

private:
    Ship(ALLEGRO_BITMAP* bitmap, int x, int y);
    void update_util(Keyboard* keyboard, int up, int down,
                     int left, int right, int shoot);

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

    int speed;
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
    Ship_factory(ALLEGRO_BITMAP* sheet);
    Ship* createShip(int x, int y)    {
        return new Ship(shipImg, x, y);
    }

    ~Ship_factory(){
        al_destroy_bitmap(shipImg);
    }

private:
    //sprite keep copy of a common sprite and should be destroyed by user
    ALLEGRO_BITMAP* shipImg;
    int shipImage_x, shipImage_y, shipImage_w, shipImage_h;
};

#endif // SHIP_H

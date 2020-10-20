#ifndef ALIEN_H
#define ALIEN_H
#include "utilities.h"
#include "bullet.h"

class Alien_Factory;
class Bullet_Maintainer;
class Alien_Maintainer;

struct Alien_image {
    ALLEGRO_BITMAP* alienBitmap;
    Object_type type;
    Alien_image(ALLEGRO_BITMAP* bitmap, Object_type t):
        alienBitmap(bitmap), type(t){};
};

struct Explosion{
    const int numOfExplosionFrames = 4;
    ALLEGRO_BITMAP** explosion_array;
    int counter = numOfExplosionFrames;
    int pos_x, pos_y;

    Explosion(ALLEGRO_BITMAP** bm_array,int x, int y): explosion_array(bm_array),
                                                        pos_x(x), pos_y(y){}
    void draw(){
        al_draw_bitmap(explosion_array[numOfExplosionFrames-counter], pos_x, pos_y, 0);
        --counter;
    }

    bool exploded(){
        return (!counter);
    }
};

class Alien : public ShootableObject {
    friend class Alien_Factory;
public:
    void draw(){
        if (alive() && (!gotHit))
            al_draw_bitmap(bug_img, pos_x, pos_y, 0);
        gotHit = false;
    }

    bool readyToFire(){
        if (fireNow)
        {
            fireNow = false;
            //give some chances to missfire
            if ((rand() % 3))
            {
                shootSound();
                return true;
            }
        }
        return false;
    }

    void update();

    bool alive(){
        return endurance;
    }

    void explodeSound(){
        if (type == THICCBOI)
            al_play_sample(sample_explode[1],0.75,0,1, ALLEGRO_PLAYMODE_ONCE,NULL);
        else
            al_play_sample(sample_explode[0],0.75,0,1.4, ALLEGRO_PLAYMODE_ONCE,NULL);
    }

    void bulletHit(){
        --endurance;
        gotHit = true;
    }

    int getScoreValue(){
        return scoreValue;
    }

protected:
    Alien(ALLEGRO_BITMAP* bitmap):  bug_img(bitmap){
        type = ALIEN;
        width = al_get_bitmap_width(bitmap);
        height = al_get_bitmap_height(bitmap);
        pos_x = between(0, DISPLAY_W - width);
        pos_y = -height;
    }

    void shootSound(){
        al_play_sample(shot_sample,0.6,0,0.7, ALLEGRO_PLAYMODE_ONCE,NULL);
    }

    int scoreValue = 0;
    ALLEGRO_BITMAP* bug_img;
    ALLEGRO_BITMAP* explosion[3];
    //two kind of speed allow: (x,1) give x pixel per frame and (1, y) with
    // y != 1, give speed 1 pixel per y frame.
    std::pair<int,int> speed;
    int speedCounter=0; //got a bug when not giving speedCounter a initial value
    int endurance;
    int shoot_interval; //give alien a unsteady shoting behave
    int interval_counter = 1;
};

class Bug_alien : public Alien {
    friend class Alien_Factory;
    Bug_alien(ALLEGRO_BITMAP* bitmap): Alien(bitmap){
        speed = std::pair<int,int>(1,2); //speed 1/2 pixel per frame
        fireWait = 6;
        endurance = 3;
        fireCountdown = fireWait;
        type = BUG;
        shoot_interval = 3;
        scoreValue = 200;
    }
};

class Arrow_alien: public Alien {
    friend class Alien_Factory;
    Arrow_alien(ALLEGRO_BITMAP* bitmap) : Alien(bitmap){
        speed = std::pair<int,int>(1,1); //speed 1 pixel per frame
        fireWait = 8;
        endurance = 2;
        fireCountdown = fireWait;
        type = ARROW;
        shoot_interval = 2;
        scoreValue = 150;
    }
};

class Thiccboi_alien: public Alien {
    friend class Alien_Factory;
    Thiccboi_alien(ALLEGRO_BITMAP* bitmap) : Alien(bitmap){
        speed = std::pair<int,int>(1,3); //speed 1/3 pixel per frame
        fireWait = 10;
        endurance = 9;
        fireCountdown = fireWait;
        type = THICCBOI;
        shoot_interval = 4;
        scoreValue = 800;
    }
};

class Alien_Factory{
public:
    Alien* createAlien(std::vector<Alien_image>& vec_AlienImage, Object_type type);

private:
    ALLEGRO_BITMAP* chooseAlien (std::vector<Alien_image>& vec_AlienImage,
                                  Object_type type);
};




#endif // ALIEN_H

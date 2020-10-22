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
    int fixFramerate;

    Explosion(ALLEGRO_BITMAP** bm_array,int x, int y);
    void draw();

    bool exploded(){
        return (!counter);
    }
};

class Alien : public ShootableObject {
    friend class Alien_Factory;
public:
    void draw();

    bool readyToFire();

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
        ALLEGRO_BITMAP* displaying = al_get_target_bitmap();
        int appearRange = al_get_bitmap_width(displaying) - width;
        int narrowRangeID = between(0,6);
        pos_x = between(narrowRangeID*(appearRange/6), (narrowRangeID+1)*
                                                        appearRange/6);
        pos_y = -height;
    }

    virtual void alien_setup()=0;

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
    int blink_counter = 0;
};

class Bug_alien : public Alien {
    friend class Alien_Factory;
    Bug_alien(ALLEGRO_BITMAP* bitmap): Alien(bitmap){
        alien_setup();
    }

    void alien_setup();
};

class Arrow_alien: public Alien {
    friend class Alien_Factory;
    Arrow_alien(ALLEGRO_BITMAP* bitmap) : Alien(bitmap){
        alien_setup();
    }

    void alien_setup();
};

class Thiccboi_alien: public Alien {
    friend class Alien_Factory;
    Thiccboi_alien(ALLEGRO_BITMAP* bitmap) : Alien(bitmap){
        alien_setup();
    }

    void alien_setup();
};

class Alien_Factory{
public:
    Alien* createAlien(std::vector<Alien_image>& vec_AlienImage, Object_type type);

private:
    ALLEGRO_BITMAP* chooseAlien (std::vector<Alien_image>& vec_AlienImage,
                                  Object_type type);
};




#endif // ALIEN_H

#include "alien.h"

extern ALLEGRO_CONFIG* config;
extern int PRIM_DISPLAY_W, PRIM_DISPLAY_H;
extern int EFFECTIVE_DISPLAY_DIAG;
extern float FRAMERATEMULTIPLIER;

Alien* Alien_Factory::createAlien(std::vector<Alien_image>& vec_AlienImage, Object_type type){
    ALLEGRO_BITMAP* alienBitmap;
    alienBitmap = chooseAlien(vec_AlienImage, type);
    must_init(alienBitmap, "choose alien");
    switch (type){
    case SHIP: //shouldn't
        return nullptr;

    case THICCBOI:
        return new Thiccboi_alien(alienBitmap);

    case ARROW:
        return new Arrow_alien(alienBitmap);

    case ALIEN:
    case BUG:
        return new Bug_alien(alienBitmap);
    }
    //not above types? just return a bug?
    return new Bug_alien(alienBitmap);
}

ALLEGRO_BITMAP* Alien_Factory::chooseAlien (std::vector<Alien_image>& vec_AlienImage,
                              Object_type type){
    auto iter = vec_AlienImage.begin();
    while (iter != vec_AlienImage.end()){
        if (iter->type == type)
            return iter->alienBitmap;
        ++iter;
    }
    //shouldn't come here
    std::cout << "found no required image for alien" << std::endl;
    return nullptr;
}

void Alien::update(){
    //update position
    if (speed.second == 1)
    {
        pos_y += speed.first*(EFFECTIVE_DISPLAY_DIAG/(1.4*240));
    }
    //speed is a fraction:
    if (speedCounter != speed.second)
        ++speedCounter;
    else {
             pos_y += EFFECTIVE_DISPLAY_DIAG/(1.4*240);
             speedCounter = 1;
    }
    //update firing state
    if (fireCountdown > 0)
    {
        --fireCountdown;
        std::cout << fireCountdown << std::endl;
    }
    else
    {
        fireCountdown = fireWait * shoot_interval;

        fireNow = true;
    }
    return;
}

bool Alien::readyToFire(){
    if (fireNow)
    {
        fireNow = false;
        static int bucketSize = RAND_MAX / 100;
        //give some chance to missfire
        if (rand() > 20*bucketSize)
        {
            shootSound();
            return true;
        }
    }
    return false;
}

void Bug_alien::alien_setup(){
    speed = std::pair<int,int>(1,2); //speed 1/2 pixel per frame
    fireWait = 8*FRAMERATEMULTIPLIER;
    endurance = 3;
    fireCountdown = fireWait;
    type = BUG;
    shoot_interval = 3;
    scoreValue = 200;
}

void Arrow_alien::alien_setup(){
    speed = std::pair<int,int>(1,1); //speed 1 pixel per frame
    fireWait = 10*FRAMERATEMULTIPLIER;
    endurance = 2;
    fireCountdown = fireWait;
    type = ARROW;
    shoot_interval = 2;
    scoreValue = 150;
}

void Thiccboi_alien::alien_setup(){
    speed = std::pair<int,int>(1,3); //speed 1/3 pixel per frame
    fireWait = 12*FRAMERATEMULTIPLIER;
    endurance = 9;
    fireCountdown = fireWait;
    type = THICCBOI;
    shoot_interval = 4;
    scoreValue = 800;
}

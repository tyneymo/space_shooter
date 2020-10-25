#include "alien.h"

extern ALLEGRO_CONFIG* config;
extern int PRIM_DISPLAY_W, PRIM_DISPLAY_H;
extern int EFFECTIVE_DISPLAY_DIAG;
extern int EFFECTIVE_DISPLAY_HEIGHT;
extern float FRAMERATEMULTIPLIER;

Explosion::Explosion(ALLEGRO_BITMAP** bm_array,int x, int y): explosion_array(bm_array),
    pos_x(x), pos_y(y){
    explosion_array = bm_array;
    pos_x = x;
    pos_y = y;
    fixFramerate = 2*FRAMERATEMULTIPLIER;
}

void Explosion::draw(){
    int w = al_get_bitmap_width(explosion_array[numOfExplosionFrames-counter]);
    int h = al_get_bitmap_height(explosion_array[numOfExplosionFrames-counter]);
    al_draw_rotated_bitmap(explosion_array[numOfExplosionFrames-counter],
            w/2, h/2, pos_x, pos_y, 0,0);
    if (!fixFramerate--){
        --counter;
        fixFramerate = 2*FRAMERATEMULTIPLIER;
    }
}

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
        pos_y += speed.first*(EFFECTIVE_DISPLAY_HEIGHT/240);
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
    }
    else
    {
        fireCountdown = fireWait;

        fireNow = true;
    }
    //update blink_counter if neccessary
    if (gotHit && !(blink_counter--)){
        gotHit = false;
        blink_counter =  6*FRAMERATEMULTIPLIER;
    }
    //for first run only
    if (!gotHit && !blink_counter) blink_counter = 6*FRAMERATEMULTIPLIER;
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

void Alien::draw(){
    if (alive() && (!gotHit))
        al_draw_bitmap(bug_img, pos_x, pos_y, 0);
    if (gotHit && ((int)(blink_counter / (2*FRAMERATEMULTIPLIER)) % 2))
        al_draw_bitmap(bug_img, pos_x, pos_y, 0);
}

void Bug_alien::alien_setup(){
    speed = std::pair<int,int>(1,2); //speed 1/2 pixel per frame
    fireWait = 35*FRAMERATEMULTIPLIER;
    endurance = 3;
    fireCountdown = fireWait;
    type = BUG;
    scoreValue = 200;
}

void Arrow_alien::alien_setup(){
    speed = std::pair<int,int>(1,1); //speed 1 pixel per frame
    fireWait = 45*FRAMERATEMULTIPLIER;
    endurance = 2;
    fireCountdown = fireWait;
    type = ARROW;
    scoreValue = 150;
}

void Thiccboi_alien::alien_setup(){
    speed = std::pair<int,int>(1,3); //speed 1/3 pixel per frame
    fireWait = 80*FRAMERATEMULTIPLIER;
    endurance = 9;
    fireCountdown = fireWait;
    type = THICCBOI;
    scoreValue = 800;
}

#include "alien.h"

Alien* Alien_Factory::createAlien(std::vector<Alien_image>& vec_AlienImage,
                                  Alien_subtype subtype){
    ALLEGRO_BITMAP* alienBitmap;
    alienBitmap = chooseAlien(vec_AlienImage, subtype);
    must_init(alienBitmap, "choose alien");
    switch (subtype){
    case THICCBOI:
        return new Thiccboi_alien(alienBitmap);

    case ARROW:
        return new Arrow_alien(alienBitmap);

    case BUG:
        return new Bug_alien(alienBitmap);
    }
    //not above types? just return a bug?
    return new Bug_alien(alienBitmap);
}

ALLEGRO_BITMAP* Alien_Factory::chooseAlien (std::vector<Alien_image>& vec_AlienImage,
                              Alien_subtype subtype){
    auto iter = vec_AlienImage.begin();
    while (iter != vec_AlienImage.end()){
        if (iter->type == subtype)
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
        pos_y += speed.first;
    }
    //speed is a fraction:
    if (speedCounter != speed.second)
        ++speedCounter;
    else {
             pos_y += 1;
             speedCounter = 1;
    }
    //update firing state
    if (fireCountdown > 0)
        --fireCountdown;
    else
    {
        ++interval_counter;
        fireCountdown = fireWait;
        if (!(interval_counter % shoot_interval))
        {
            fireNow = true;
        }
    }
    return;
}

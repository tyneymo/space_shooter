#ifndef ALIEN_H
#define ALIEN_H
#include "utilities.h"
#include <list>
#include <memory>
#include <iostream>
#include <vector>
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
    friend void shotAndHit(Alien_Maintainer* alienMtn, Bullet_Maintainer* bulletMtn);
public:
    void draw(){
        if (alive())
            al_draw_bitmap(bug_img, pos_x, pos_y, 0);
    }

    void update(){
        if (speed.second == 1)
        {
            pos_y += speed.first;
            return;
        }
        //speed is a fraction:
        if (speedCounter != speed.second)
            ++speedCounter;
        else {
                 pos_y += 1;
                 speedCounter = 1;
        }

        if (fireCountdown > 0)
            --fireCountdown;
        else
            fireNow = true;
        return;
    }

    bool alive(){
        return endurance;
    }

protected:
    Alien(ALLEGRO_BITMAP* bitmap):  bug_img(bitmap){
        type = ALIEN;
        width = al_get_bitmap_width(bitmap);
        height = al_get_bitmap_height(bitmap);
        pos_x = between(0, DISPLAY_W * SCALE);
        pos_y = between(-30,0);
    }



    ALLEGRO_BITMAP* bug_img;
    ALLEGRO_BITMAP* explosion[3];
    //two kind of speed allow: (x,1) give x pixel per frame and (1, y) with
    // y != 1, give speed 1 pixel per y frame.
    std::pair<int,int> speed;
    int speedCounter=0; //got a bug when not giving speedCounter a initial value
    int endurance;
};

class Bug_alien : public Alien {
    friend class Alien_Factory;
    Bug_alien(ALLEGRO_BITMAP* bitmap): Alien(bitmap){
        speed = std::pair<int,int>(1,1); //speed 1 pixel per frame
        fireWait = 6;
        endurance = 3;
        fireCountdown = fireWait;
    }
};

class Arrow_alien: public Alien {
    friend class Alien_Factory;
    Arrow_alien(ALLEGRO_BITMAP* bitmap) : Alien(bitmap){
        speed = std::pair<int,int>(2,1); //speed 2 pixel per frame
        fireWait = 8;
        endurance = 2;
        fireCountdown = fireWait;
    }
};

class Thiccboi_alien: public Alien {
    friend class Alien_Factory;
    Thiccboi_alien(ALLEGRO_BITMAP* bitmap) : Alien(bitmap){
        speed = std::pair<int,int>(1,2); //speed 0.5 pixel per frame
        fireWait = 10;
        endurance = 6;
        fireCountdown = fireWait;
    }
};



class Alien_Factory{
public:
    Alien* createAlien(std::vector<Alien_image>& vec_AlienImage, Object_type type){
        ALLEGRO_BITMAP* alienBitmap;
        alienBitmap = chooseAlien(vec_AlienImage, type);
        must_init(alienBitmap, "choose alien");
        switch (type){
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

private:
    ALLEGRO_BITMAP* chooseAlien (std::vector<Alien_image>& vec_AlienImage,
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
};

class Alien_Maintainer{
    friend void shotAndHit();
public:
    Alien_Maintainer(Alien_Factory* factory, ALLEGRO_BITMAP* spritesheet) :
                        alienFactory(factory), sprite(spritesheet){
        ALLEGRO_BITMAP* bitmapPtr;
        int x,y,w,h;
        //BUG:
        x = 19, y = 0, w = 14, h = 9;
        bitmapPtr = sprite_grab(sprite, x,y,w,h);
        alienImages.push_back(Alien_image(bitmapPtr, Object_type::BUG));
        //ARROW:
        x = 19, y = 10, w = 13, h = 10;
        bitmapPtr = sprite_grab(sprite, x,y,w,h);
        alienImages.push_back(Alien_image(bitmapPtr, Object_type::ARROW));
        //THICCBOI:
        x = 0, y = 21, w = 45, h = 27;
        bitmapPtr = sprite_grab(sprite, x,y,w,h);
        alienImages.push_back(Alien_image(bitmapPtr, Object_type::THICCBOI));
        //explostion frames effect, extracts from sprites
        explosion_array[0] = sprite_grab(sprite, 33,10,9,9);
        explosion_array[1] = sprite_grab(sprite, 43,9,11,11);
        explosion_array[2] = sprite_grab(sprite, 46,21,17,18);
        explosion_array[3] = sprite_grab(sprite, 46,40,17,17);

    }
    void add(Object_type type){
        alienList.push_back(std::shared_ptr<Alien>
                            (alienFactory->createAlien(alienImages, type)));
    }

    //add a random Alien
    void add(){
        Object_type alienArray[] = {BUG, ARROW, THICCBOI, BUG, ARROW};
        Object_type alienType = alienArray[rand() % (sizeof(alienArray) /
                                                     sizeof(Object_type))];
        alienList.push_back(std::shared_ptr<Alien>
                            (alienFactory->createAlien(alienImages, alienType)));
    }

    void maintain(Bullet_Maintainer* bulletMaintainer){
        auto iter = alienList.begin();
        while (iter != alienList.end()){
            auto local_iter = iter;
            iter++;
            auto ptr = *local_iter;
            ptr->update();
//            ptr->checkBulletHit(bulletMaintainer);
            if (ptr->getLocation().second > DISPLAY_H * SCALE )
                alienList.erase(local_iter);
            if (!ptr->alive())
            {
                explosion_list.push_back(Explosion(explosion_array,
                                        ptr->getLocation().first +
                                        ptr->getDimension().first/2,
                                         ptr->getLocation().second +
                                         ptr->getDimension().second/2));
                alienList.erase(local_iter);
            }
            if (ptr->readyToFire())
                bulletMaintainer->add(&(*ptr));
        }
    }

    void draw(){
        auto iter = alienList.begin();
        while (iter != alienList.end()){
            auto local_iter = iter;
            iter++;
            auto ptr = *local_iter;
            ptr->draw();
        }
        auto explodeIter = explosion_list.begin();
        while (explodeIter != explosion_list.end())
        {
            auto local_iter = explodeIter;
            explodeIter++;
            if (local_iter->exploded())
                explosion_list.erase(local_iter);
            else local_iter->draw();
        }
    }

    std::list<std::shared_ptr<Alien>>::iterator begin(){
        return alienList.begin();
    }

    std::list<std::shared_ptr<Alien>>::iterator end(){
        return alienList.end();
    }

    ~Alien_Maintainer(){
        //clean all image except sprite, which is common to ship, bullet and /
        //aliens
        auto vec_iter = alienImages.begin();
        while (vec_iter++ != alienImages.end())
            al_destroy_bitmap(vec_iter->alienBitmap);
        for (int i = 0; i < sizeof(explosion_array) / sizeof(ALLEGRO_BITMAP*);
             ++i)
            al_destroy_bitmap(explosion_array[i]);
    }

private:
    Alien_Factory* alienFactory;
    ALLEGRO_BITMAP* sprite;
    ALLEGRO_BITMAP* explosion_array[4];
    std::vector<Alien_image> alienImages;
    std::list<Explosion> explosion_list;
    std::list<std::shared_ptr<Alien>> alienList;
};


#endif // ALIEN_H

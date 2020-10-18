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
//    enum alienImageType {BUG_IMG, ARROW_IMG, THICCBOI_IMG, EXPLOSION1,
//                         EXPLOSION2, EXPLOSION3};
    ALLEGRO_BITMAP* alienBitmap;
    Object_type type;
    Alien_image(ALLEGRO_BITMAP* bitmap, Object_type t):
        alienBitmap(bitmap), type(t){};
//    ~Alien_image(){
//        al_destroy_bitmap(alienBitmap);
//    }
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
        return;
    }

    bool alive(){
        return endurance;
    }

protected:
    Alien(ALLEGRO_BITMAP* bitmap, int w, int h):  bug_img(bitmap){
        type = ALIEN;
        width = w;
        height = h;
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
    Bug_alien(ALLEGRO_BITMAP* bitmap, int w, int h): Alien(bitmap, w, h){
        speed = std::pair<int,int>(1,1); //speed 1 pixel per frame
        fireWait = 6;
        endurance = 3;
    }
};

class Arrow_alien: public Alien {
    friend class Alien_Factory;
    Arrow_alien(ALLEGRO_BITMAP* bitmap, int w, int h) : Alien(bitmap, w, h){
        speed = std::pair<int,int>(2,1); //speed 2 pixel per frame
        fireWait = 8;
        endurance = 2;
    }
};

class Thiccboi_alien: public Alien {
    friend class Alien_Factory;
    Thiccboi_alien(ALLEGRO_BITMAP* bitmap, int w, int h) : Alien(bitmap, w, h){
        speed = std::pair<int,int>(1,2); //speed 0.5 pixel per frame
        pos_x = between(0, DISPLAY_W * SCALE);
        pos_y = between(-50,0);
        fireWait = 10;
        endurance = 6;
    }
};



class Alien_Factory{
public:
    Alien* createAlien(std::vector<Alien_image>& vec_AlienImage, Object_type type){
        Alien* returnAlien;
        switch (type){
        case THICCBOI:
//make a function to locate image based on type
            break;

        case ARROW:
            //need to change to configurable
            AlienImage_x = 19;
            AlienImage_y = 10;
            AlienImage_w = 13;
            AlienImage_h = 10;
            alienImg = sprite_grab(sprite, AlienImage_x,
                                                   AlienImage_y,
                                                   AlienImage_w,
                                                   AlienImage_h);
            returnAlien = new Arrow_alien(alienImg, AlienImage_w, AlienImage_h);
            break;

        case BUG:
            //make these hard code number some constant or configurable
            AlienImage_x = 19;
            AlienImage_y = 0;
            AlienImage_w = 14;
            AlienImage_h = 9;
            alienImg = sprite_grab(sprite, AlienImage_x,
                                                   AlienImage_y,
                                                   AlienImage_w,
                                                   AlienImage_h);
            returnAlien = new Bug_alien(alienImg, AlienImage_w, AlienImage_h);
            break;
        }

        return returnAlien;
    }

private:
//    ALLEGRO_BITMAP* chooseAlien (std::vector<Alien_image>& vec_AlienImage,
//                                  Object_type type){
//        auto iter = vec_AlienImage.begin();
//        while (iter != vec_AlienImage.end()){

//            ++iter;
//        }
//    }
    ALLEGRO_BITMAP* sprite;
    //may need to find a way to make these image pos read from config file
};

class Alien_Maintainer{
    friend void shotAndHit();
public:
    Alien_Maintainer(Alien_Factory* factory) : alienFactory(factory){
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
        x = 19, y = 0, w = 14, h = 9;
        bitmapPtr = sprite_grab(sprite, x,y,w,h);
        alienImages.push_back(Alien_image(bitmapPtr, Object_type::THICCBOI));
    }
    void add(Object_type type){
        alienList.push_back(std::shared_ptr<Alien>
                            (alienFactory->createAlien(type)));
    }

    //add a random Alien
    void add(){
        Object_type alienArray[] = {BUG, ARROW, THICCBOI, BUG, ARROW};
        Object_type alienType = alienArray[rand() % (sizeof(alienArray) /
                                                     sizeof(Object_type))];
        alienList.push_back(std::shared_ptr<Alien>
                            (alienFactory->createAlien(alienType)));
    }

    void maintain(){
        auto iter = alienList.begin();
        while (iter != alienList.end()){
            auto local_iter = iter;
            iter++;
            auto ptr = *local_iter;
            ptr->update();
//            ptr->checkBulletHit(bulletMaintainer);
            if (ptr->getLocation().second > DISPLAY_H * SCALE || (!ptr->alive()))
            {
                alienList.erase(local_iter);
                std::cout << "erased an alien"    << std::endl;
            }
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
    }

private:
    Alien_Factory* alienFactory;
    ALLEGRO_BITMAP* sprite;
    ALLEGRO_BITMAP* explosion[3];
    std::vector<Alien_image> alienImages;
    std::list<std::shared_ptr<Alien>> alienList;
};


#endif // ALIEN_H

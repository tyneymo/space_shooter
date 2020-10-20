#ifndef MAINTAINERS_H
#define MAINTAINERS_H
#include "alien.h"
#include "bullet.h"
#include <list>
#include <memory>

class Alien_Factory;

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

    void maintain(Bullet_Maintainer* bulletMaintainer, Score* score);

    void draw();

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
        for (unsigned int i = 0; i < sizeof(explosion_array) / sizeof(ALLEGRO_BITMAP*);
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

class Bullet_Maintainer{
    friend void shotAndHit();
public:
    friend class Alien;
    friend class Ship;
    friend class Alien_Maintainer;
    typedef std::list<std::shared_ptr<Bullet>>::iterator iterator;

    Bullet_Maintainer(Bullet_factory* factory, ALLEGRO_BITMAP* spritesheet):
                        bulletFactory(factory), sprite(spritesheet){
        ALLEGRO_BITMAP* bmp;
        int x, y, w, h;
        //SHIP:
        x = 13, y = 0, w = 2, h = 9;
        bmp = sprite_grab(spritesheet, x, y, w, h);
        bulletImages.push_back(Bullet_image(bmp, SHIP));
        //ALIEN:
        x = 13, y = 10, w = 4, h = 4;
        bmp = sprite_grab(spritesheet, x, y, w, h);
        bulletImages.push_back(Bullet_image(bmp, ALIEN));
        //BUG:
        bulletImages.push_back(Bullet_image(bmp, BUG));
        //ARROW:
        bulletImages.push_back(Bullet_image(bmp, ARROW));
        //THICCBOI:
        bulletImages.push_back(Bullet_image(bmp, THICCBOI));


        //initialize images in spark_array:
        spark_array[0] = sprite_grab(sprite, 34,0,10,8);
        spark_array[1] = sprite_grab(sprite, 45,0,7,8);
        spark_array[2] = sprite_grab(sprite, 54,0,9,8);
    }

    void add(ShootableObject* shooter);

    void maintain(ShootableObject* ship, Alien_Maintainer* alienMaintainer);

    iterator begin(){
        return bullet_list.begin();
    }

    iterator end(){
        return bullet_list.end();
    }

    void draw();

    int getBulletNumber(){
        return bullet_list.size();
    }

    ~Bullet_Maintainer(){
        for (unsigned int i  = 0; i < sizeof (spark_array) / sizeof(ALLEGRO_BITMAP*); ++i)
            al_destroy_bitmap(spark_array[i]);
    }

private:
    std::list<std::shared_ptr<Bullet>> bullet_list;
    Bullet_factory* bulletFactory;
    ALLEGRO_BITMAP* spark_array[3];
    ALLEGRO_BITMAP* sprite;
    std::vector<Bullet_image> bulletImages;
    std::list<BulletSpark> spark_list;
    int bulletCreated = 0;
    int bulletDestroyed = 0;
};



#endif // UTIL2_H

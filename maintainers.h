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
    Alien_Maintainer(Alien_Factory* factory, ALLEGRO_BITMAP* spritesheet);
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
                auto save = vec_iter->alienBitmap;
                al_destroy_bitmap(vec_iter->alienBitmap);
                while (++vec_iter != alienImages.end())
                {
                    if (vec_iter->alienBitmap != save)
                        al_destroy_bitmap(vec_iter->alienBitmap);
                    save = vec_iter->alienBitmap;
                }
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

    Bullet_Maintainer(Bullet_factory* factory, ALLEGRO_BITMAP* spritesheet);

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
        auto vec_iter = bulletImages.begin();
                auto save = vec_iter->bulletBitmap;
                al_destroy_bitmap(vec_iter->bulletBitmap);
                while (++vec_iter != bulletImages.end())
                {
                    if (vec_iter->bulletBitmap != save)
                        al_destroy_bitmap(vec_iter->bulletBitmap);
                    save = vec_iter->bulletBitmap;
                }
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



#endif // MAINTAINER_H

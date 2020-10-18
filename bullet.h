#ifndef BULLET_H
#define BULLET_H
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <cstdlib>
#include <iostream>
#include <list>
#include <memory>
#include "utilities.h"
class Bullet_factory;
class Alien;
class Ship;

struct BulletSpark{
    const int numOfSparkFrames = 3;
    ALLEGRO_BITMAP** spark_array;
    int counter = numOfSparkFrames;
    int pos_x, pos_y;

    BulletSpark(ALLEGRO_BITMAP** bm_array,int x, int y): spark_array(bm_array),
                                                        pos_x(x), pos_y(y){}
    void draw(){
        al_draw_bitmap(spark_array[numOfSparkFrames-counter], pos_x, pos_y, 0);
        --counter;
    }

    bool sparked(){
        return (!counter);
    }
};

class Bullet{
public:
    ~Bullet(){
        al_destroy_bitmap(bullet_img);
    }

    bool ifActive(){
        return active;
    }

    void setActivationState(bool state){
        active = state;
    }

    //this function should be override in bullet for ship and for alien
    void update(){
        pos_x += speed_x;
        pos_y += speed_y;
    }

    std::pair<int,int> getLocation(){
            return std::pair<int,int> (pos_x, pos_y);
        }

    void draw(){
        if (active)
            al_draw_bitmap(bullet_img, pos_x, pos_y, 0);
    }

    std::tuple<int,int,int,int> getBulletInfo(){
        return std::make_tuple(pos_x,pos_y,width,height);
    }

    Object_type shooterType(){
        return bulletSource;
    }


protected:
    //shooter->getLocation().first + shooter->getDimension().first/2) to center
    //the bullet in between shooter object.
    Bullet(ALLEGRO_BITMAP* bitmap, ShootableObject* shooter)
            : bullet_img(bitmap),
              pos_x(shooter->getLocation().first + shooter->getDimension().first/2),
              pos_y(shooter->getLocation().second),
              width(shooter->getDimension().first),
              height(shooter->getDimension().second){
                active = true;
            }
    ALLEGRO_BITMAP* bullet_img;
    int pos_x, pos_y, width, height;
    int speed_x=0, speed_y=0;
    int strength = 1;
    bool active = true;
    Object_type bulletSource;
};

class Ship_bullet: public Bullet{
public:
    Ship_bullet (ALLEGRO_BITMAP* bitmap, ShootableObject* shooter)
        : Bullet(bitmap,shooter){
        Bullet::speed_x = 0;
        Bullet::speed_y = -4;
        bulletSource = SHIP;
    }
};

class Bullet_factory{

public:
    const char* bullet_type[3] = {"shipshot1", "shipshot2", "alien"};
    Bullet_factory(ALLEGRO_BITMAP* bitmap): sheet(bitmap){}

    Bullet* createBullet(ShootableObject* shooter){
        ALLEGRO_BITMAP* bulletImg;
        int bulletImg_x, bulletImg_y, bulletImg_w, bulletImg_h;
        switch (shooter->getType()){
        case SHIP:
            bulletImg_x = 13;
            bulletImg_y = 0;
            bulletImg_w = 2;
            bulletImg_h = 9;
            bulletImg = sprite_grab(sheet, bulletImg_x, bulletImg_y,
                                    bulletImg_w, bulletImg_h);
            break;
//        case SHIP_BULLET2:
//            bulletImg_x = 16;
//            bulletImg_y = 0;
//            bulletImg_w = 2;
//            bulletImg_h = 9;
//            bulletImg = sprite_grab(sheet, bulletImg_x, bulletImg_y,
//                                    bulletImg_w, bulletImg_h);
//            break;

        case ALIEN:
            break;
        }
        return new Ship_bullet(bulletImg, shooter);
    }

private:
    ALLEGRO_BITMAP* sheet;
};

class Bullet_Maintainer{
    friend void shotAndHit();
public:
    friend class Alien;
    friend class Ship;
    typedef std::list<std::shared_ptr<Bullet>>::iterator iterator;
    Bullet_Maintainer(Bullet_factory* factory, ALLEGRO_BITMAP* spritesheet):
                        bulletFactory(factory), sprite(spritesheet){
        //initialize images in spark_array:
        spark_array[0] = sprite_grab(sprite, 34,0,10,8);
        spark_array[1] = sprite_grab(sprite, 45,0,7,8);
        spark_array[2] = sprite_grab(sprite, 54,0,9,8);
    }

    void add(ShootableObject* shooter){
            std::shared_ptr<Bullet> bulletPtr(bulletFactory->createBullet(shooter));
            bullet_list.push_back(bulletPtr);
            std::cout << "added a bullet" << std::endl;
    }

    void maintain(){
        auto iter = bullet_list.begin();
        while (iter != bullet_list.end()){
            auto local_iter = iter;
            iter++;
            auto ptr = *local_iter;
            ptr->update();
            if (ptr->getLocation().second < 0 ||
                ptr->getLocation().second > DISPLAY_H * SCALE)
                bullet_list.erase(local_iter);
            if (!ptr->ifActive()){
                spark_list.push_back(BulletSpark(spark_array,
                                                  std::get<0>((*ptr).getBulletInfo()),
                                                  std::get<1>((*ptr).getBulletInfo())));
                bullet_list.erase(local_iter);
            }
        }
    }

    iterator begin(){
        return bullet_list.begin();
    }

    iterator end(){
        return bullet_list.end();
    }

    void draw(){
        auto bullet_iter = bullet_list.begin();
        while (bullet_iter != bullet_list.end()){
            auto local_iter = bullet_iter;
            bullet_iter++;
            auto ptr = *local_iter;
            if (ptr->ifActive())
                ptr->draw();
        }
        auto spark_iter = spark_list.begin();
        while (spark_iter != spark_list.end()){
            auto local_iter = spark_iter;
            spark_iter++;
            if (!local_iter->sparked())
                local_iter->draw();
        }
    }

private:
    std::list<std::shared_ptr<Bullet>> bullet_list;
    Bullet_factory* bulletFactory;
    ALLEGRO_BITMAP* spark_array[3];
    ALLEGRO_BITMAP* sprite;
    std::list<BulletSpark> spark_list;
};

#endif

#ifndef BULLET_H
#define BULLET_H
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <cstdlib>
#include <iostream>
#include <list>
#include <vector>
#include <memory>
#include "utilities.h"
class Bullet_factory;
class Bullet_Maintainer;
class Alien;
class Ship;
class Alien_Maintainer;

struct Bullet_image {
    ALLEGRO_BITMAP* bulletBitmap;
    Object_type type;
    Bullet_image(ALLEGRO_BITMAP* bitmap, Object_type t):
        bulletBitmap(bitmap), type(t){};
};

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
    friend class Bullet_Maintainer;
public:

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
        if (active){
            if (bulletSource != SHIP){
                int flashing = (flashEffect++ / 2) % 2;
                ALLEGRO_COLOR tint = flashing? al_map_rgb_f(1,1,1) :
                                               al_map_rgb_f(0.2,0.2,0.2);
                al_draw_tinted_bitmap(bullet_img, tint, pos_x, pos_y, 0);
            }
            al_draw_bitmap(bullet_img, pos_x, pos_y, 0);
        }
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
              pos_y(shooter->getLocation().second + shooter->getDimension().second),
              width(al_get_bitmap_width(bitmap)),
              height(al_get_bitmap_height(bitmap)){}

    ALLEGRO_BITMAP* bullet_img;
    int pos_x, pos_y, width, height;
    int speed_x, speed_y;
    bool active = true;
    Object_type bulletSource;
    int flashEffect = 0;
};

//class Alien_bullet: public Bullet{
//public:
//    Alien_bullet (ALLEGRO_BITMAP* bitmap, ShootableObject* shooter)
//        : Bullet(bitmap,shooter){
//        Bullet::speed_x = 0;
//        Bullet::speed_y = -2;
//        bulletSource = ALIEN;
//    }
//};

class Ship_bullet: public Bullet{
public:
    Ship_bullet (ALLEGRO_BITMAP* bitmap, ShootableObject* shooter)
        : Bullet(bitmap,shooter){
        Bullet::speed_x = 0;
        Bullet::speed_y = -4;
        bulletSource = SHIP;
    }
};

class Bug_bullet: public Bullet{
public:
    Bug_bullet(ALLEGRO_BITMAP* bitmap, ShootableObject* shooter)
        : Bullet(bitmap, shooter){
        speed_x = between(-2, 2);
        speed_y = between(-1, 3);
        bulletSource = ALIEN;
    }

};

class Arrow_bullet: public Bullet{
public:
    Arrow_bullet(ALLEGRO_BITMAP* bitmap, ShootableObject* shooter)
        : Bullet(bitmap, shooter){
        speed_x = between(-2, 2);
        speed_y = between(-1, 3);
        bulletSource = ALIEN;
    }
};

class Thiccboi_bulelt: public Bullet{
public:
    Thiccboi_bulelt(ALLEGRO_BITMAP* bitmap, ShootableObject* shooter)
        : Bullet(bitmap, shooter){
        speed_x = 0;
        speed_y = 2;
        bulletSource = ALIEN;
    }
};

class Bullet_factory{

public:
    Bullet_factory(ALLEGRO_BITMAP* bitmap): sheet(bitmap){}

    Bullet* createBullet(std::vector<Bullet_image>& vBullet,
                         ShootableObject* shooter){
        ALLEGRO_BITMAP* bulletImg = chooseBullet(vBullet, shooter);
        switch (shooter->getType()){
        case SHIP:
            return new Ship_bullet(bulletImg, shooter);
            break;

        case BUG:
            return new Bug_bullet(bulletImg, shooter);
            break;

        case ARROW:
            return new Arrow_bullet(bulletImg, shooter);
            break;

        case THICCBOI:
            return new Thiccboi_bulelt(bulletImg, shooter);
            break;
        }
        //nothing above?
        return nullptr;
    }

private:
    ALLEGRO_BITMAP* chooseBullet (std::vector<Bullet_image>& vBullet,
                                  ShootableObject* shooter){
        auto iter = vBullet.begin();
        while (iter != vBullet.end()){
            if (iter->type == shooter->getType())
                return iter->bulletBitmap;
            ++iter;
        }
        //shouldn't come here
        std::cout << "found no required image for bullet" << std::endl;
        return nullptr;
    }
    ALLEGRO_BITMAP* sheet;
};



#endif

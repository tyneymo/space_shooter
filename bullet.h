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
    int speed_x, speed_y;
    int strength = 1;
    bool active = true;
    Object_type bulletSource;
};

class Alien_bullet: public Bullet{
public:
    Alien_bullet (ALLEGRO_BITMAP* bitmap, ShootableObject* shooter)
        : Bullet(bitmap,shooter){
        Bullet::speed_x = 0;
        Bullet::speed_y = -2;
        bulletSource = SHIP;
    }
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

class Bug_bullet: public Bullet{
public:
    Bug_bullet(ALLEGRO_BITMAP* bitmap, ShootableObject* shooter)
        : Bullet(bitmap, shooter){
        speed_x = between(-2, 2);
        speed_y = between(-2, 2);
        bulletSource = ALIEN;
        std::cout << "created a bug bullet" << std::endl;
    }

};

class Arrow_bullet: public Bullet{
public:
    Arrow_bullet(ALLEGRO_BITMAP* bitmap, ShootableObject* shooter)
        : Bullet(bitmap, shooter){
        speed_x = between(-2, 2);
        speed_y = between(-2, 2);
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

    Bullet* createBullet(std::vector<Bullet_image> vBullet,
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
        x = 13, y = 10, w = 4, h = 4;
        bmp = sprite_grab(spritesheet, x, y, w, h);
        bulletImages.push_back(Bullet_image(bmp, BUG));
        //ARROW:
        x = 13, y = 10, w = 4, h = 4;
        bmp = sprite_grab(spritesheet, x, y, w, h);
        bulletImages.push_back(Bullet_image(bmp, ARROW));
        //THICCBOI:
        x = 13, y = 10, w = 4, h = 4;
        bmp = sprite_grab(spritesheet, x, y, w, h);
        bulletImages.push_back(Bullet_image(bmp, THICCBOI));


        //initialize images in spark_array:
        spark_array[0] = sprite_grab(sprite, 34,0,10,8);
        spark_array[1] = sprite_grab(sprite, 45,0,7,8);
        spark_array[2] = sprite_grab(sprite, 54,0,9,8);
    }

    void add(ShootableObject* shooter){
            auto ptr = bulletFactory->createBullet(bulletImages,shooter);
            if (ptr){
                if (ptr->speed_x == 0 && ptr->speed_y == 0)
                    return;
                std::shared_ptr<Bullet> bulletPtr(ptr);
                bullet_list.push_back(bulletPtr);
                std::cout << "added a bullet" << std::endl;
            }
            else
                std::cout << "couldn't create bullet" << std::endl;
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

    ~Bullet_Maintainer(){
        //clean images here
    }

private:
    std::list<std::shared_ptr<Bullet>> bullet_list;
    Bullet_factory* bulletFactory;
    ALLEGRO_BITMAP* spark_array[3];
    ALLEGRO_BITMAP* sprite;
    std::vector<Bullet_image> bulletImages;
    std::list<BulletSpark> spark_list;
};

#endif

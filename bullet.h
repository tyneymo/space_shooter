#ifndef BULLET_H
#define BULLET_H
#include "utilities.h"
#include <vector>
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
    ~Bullet_image(){
//        al_destroy_bitmap(bulletBitmap);
    }
};

struct BulletSpark{
    const int numOfSparkFrames = 3;
    ALLEGRO_BITMAP** spark_array;
    int counter = numOfSparkFrames;
    int pos_x, pos_y;
    int fixFramerate;

    BulletSpark(ALLEGRO_BITMAP** bm_array,int x, int y);

    void draw();

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
            return std::make_pair(pos_x, pos_y);
    }

    std::pair<int,int> getDimension(){
        return std::make_pair(width, height);
    }

    virtual void draw();

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
              width(al_get_bitmap_width(bitmap)),
              height(al_get_bitmap_height(bitmap)){
        must_init(bullet_img, "bulletimage");
    }

    ALLEGRO_BITMAP* bullet_img;
    int pos_x, pos_y, width, height;
    int speed_x, speed_y;
    bool active = true;
    Object_type bulletSource;
    int flashEffect = 0;
};

class Ship_bullet: public Bullet{
public:
    Ship_bullet (ALLEGRO_BITMAP* bitmap, ShootableObject* shooter)
        : Bullet(bitmap,shooter){
        ship_bullet_setup();
    }

private:
    virtual void ship_bullet_setup();
};

class Alien_bullet: public Bullet{
public:
    Alien_bullet(ALLEGRO_BITMAP* bitmap, ShootableObject* shooter)
        : Bullet(bitmap, shooter){
        pos_y += shooter->getDimension().second - al_get_bitmap_height(bitmap)/4;
        pos_x -= al_get_bitmap_width(bitmap)/2;
    };
protected:
    virtual void alien_bullet_setup(float, float, float, float);
};

class Bug_bullet: public Alien_bullet{
public:
    Bug_bullet(ALLEGRO_BITMAP* bitmap, ShootableObject* shooter)
        : Alien_bullet(bitmap, shooter){
        alien_bullet_setup(-2.0,2.0,-1.0,1.0);
        bulletSource = ALIEN;
    }

};

class Arrow_bullet: public Alien_bullet{
public:
    Arrow_bullet(ALLEGRO_BITMAP* bitmap, ShootableObject* shooter)
        : Alien_bullet(bitmap, shooter){
        alien_bullet_setup(-2.0,2.0,-1.0,1.0);
        bulletSource = ALIEN;
    }
};

class Thiccboi_bulelt: public Alien_bullet{
public:
    Thiccboi_bulelt(ALLEGRO_BITMAP* bitmap, ShootableObject* shooter)
        : Alien_bullet(bitmap, shooter){
        alien_bullet_setup(0,0,0,0);
        bulletSource = ALIEN;
        rotating = between(0,18);
    }

    void alien_bullet_setup(float,float,float,float); //override aliens setup

    void draw();
private:
    int rotating = 0;
};

class Bullet_factory{

public:
    Bullet_factory(){}

    Bullet* createBullet(std::vector<Bullet_image>& vBullet,
                         ShootableObject* shooter);

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
};



#endif

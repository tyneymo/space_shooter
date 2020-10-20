#include "bullet.h"
#include "alien.h"
#include "ship.h"

void Bullet::draw(){
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

Bullet* Bullet_factory::createBullet(std::vector<Bullet_image>& vBullet,
                     ShootableObject* shooter){
    ALLEGRO_BITMAP* bulletImg = chooseBullet(vBullet, shooter);
    must_init(bulletImg, "bulletimage in create function");
    switch (shooter->getType()){
    case SHIP:
        return new Ship_bullet(bulletImg, shooter);
        break;
    case ALIEN:
    Alien* alienPtr = static_cast<Alien*>(shooter);
    switch(alienPtr->getSubtype()){
        case BUG:
            return new Bug_bullet(bulletImg, shooter);
            break;

        case ARROW:
            return new Arrow_bullet(bulletImg, shooter);
            break;



        case THICCBOI:
            int oldWidth = al_get_bitmap_width(bulletImg);
            int oldHeigh = al_get_bitmap_height(bulletImg);
            //sometimes thiccboi shoot double, sometimes single row of bullets
            ALLEGRO_BITMAP* thiccboiBulletBmp = al_create_bitmap(oldWidth*3,
                                                                 oldHeigh + 2*oldHeigh*(rand() % 2));
            ALLEGRO_BITMAP* old_target = al_get_target_bitmap();
            al_set_target_bitmap(thiccboiBulletBmp);
            for (int i = 0; i<2; ++i)
                for (int j = 0; j < 2; ++j)
                    al_draw_bitmap(bulletImg, 2*i*oldWidth, 2*j*oldHeigh, 0);
            al_set_target_bitmap(old_target);
            return new Thiccboi_bulelt(thiccboiBulletBmp, shooter);
            break;
        }
    }
    //nothing above?
    std::cout << "shooter type not availabe" << std::endl;
    return nullptr;
}

void Ship_bullet::updateSubtype(ShootableObject* shooter){
    subtype = static_cast<Ship*>(shooter)->getSubtype();
}

void Alien_bullet::updateSubtype(ShootableObject* shooter){
    subtype = static_cast<Alien*>(shooter)->getSubtype();
}

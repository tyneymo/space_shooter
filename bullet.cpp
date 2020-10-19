#include "bullet.h"

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

    case BUG:
        return new Bug_bullet(bulletImg, shooter);
        break;

    case ARROW:
        return new Arrow_bullet(bulletImg, shooter);
        break;

    case ALIEN:

    case THICCBOI:
        return new Thiccboi_bulelt(bulletImg, shooter);
        break;
    }
    //nothing above?
    std::cout << "shooter type not availabe" << std::endl;
    return nullptr;
}

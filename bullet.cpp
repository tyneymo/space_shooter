#include "bullet.h"

extern ALLEGRO_CONFIG* config;
extern int PRIM_DISPLAY_W, PRIM_DISPLAY_H;
extern int EFFECTIVE_DISPLAY_DIAG;
extern float FRAMERATEMULTIPLIER;

BulletSpark::BulletSpark(ALLEGRO_BITMAP** bm_array,int x, int y) {
    spark_array = bm_array;
    pos_x = x;
    pos_y = y;
    fixFramerate = 2*FRAMERATEMULTIPLIER;
}

void BulletSpark::draw(){
        al_draw_bitmap(spark_array[numOfSparkFrames-counter], pos_x, pos_y, 0);
        if (!fixFramerate--){
            --counter;
            fixFramerate = 2*FRAMERATEMULTIPLIER;
        }
}

void Bullet::draw(){
    if (active){
        if (bulletSource != SHIP){
            int flashing = (int)(flashEffect++ / (2*FRAMERATEMULTIPLIER)) % 2;
            ALLEGRO_COLOR tint = flashing? al_map_rgba_f(1,1,1,1) :
                                           al_map_rgba_f(0.5,0.5,0.5,0.5);
            al_draw_tinted_bitmap(bullet_img, tint, pos_x, pos_y, 0);
            return;
        }
        al_draw_bitmap(bullet_img, pos_x, pos_y, 0);
    }
}

void Ship_bullet::ship_bullet_setup(){
    speed_x = 0;
    speed_y = (EFFECTIVE_DISPLAY_DIAG/(1.4*240)) * (-1.5);
    bulletSource = SHIP;
}

void Alien_bullet::alien_bullet_setup(float lim_x1, float lim_x2, float lim_y1,
                                      float lim_y2){
    speed_x = (EFFECTIVE_DISPLAY_DIAG/(1.4*320)) * between_f(lim_x1, lim_x2);
    speed_y = (EFFECTIVE_DISPLAY_DIAG/(1.4*240)) * between_f(lim_y1, lim_y2);
}

void Thiccboi_bulelt::alien_bullet_setup(int, int, int, int){
    speed_x = 0;
    speed_y = (EFFECTIVE_DISPLAY_DIAG/(1.4*240));
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
        int oldWidth = al_get_bitmap_width(bulletImg);
        int oldHeigh = al_get_bitmap_height(bulletImg);
        //sometimes thiccboi shoot double, sometimes single row of bullets
        ALLEGRO_BITMAP* thiccboiBulletBmp = al_create_bitmap(oldWidth*3,
                                            oldHeigh + 2*oldHeigh*(rand() % 2));
        ALLEGRO_BITMAP* saveDisplay = al_get_target_bitmap();
        al_set_target_bitmap(thiccboiBulletBmp);
        al_clear_to_color(al_map_rgba_f(0,0,0,0));
        for (int i = 0; i<2; ++i)
            for (int j = 0; j < 2; ++j)
                al_draw_bitmap(bulletImg, 2*i*oldWidth, 2*j*oldHeigh, 0);
        al_set_target_bitmap(saveDisplay);
        return new Thiccboi_bulelt(thiccboiBulletBmp, shooter);
        break;
    }
    //nothing above?
    std::cout << "shooter type not availabe" << std::endl;
    return nullptr;
}

#include "maintainers.h"
#include "ship.h"

extern ALLEGRO_CONFIG* config;
extern int PRIM_DISPLAY_W, PRIM_DISPLAY_H;

bool bulletObjCollide(ShootableObject* obj, Bullet* bullet){
    if (bullet->shooterType() == ALIEN && (obj->getType() == BUG
                                           || obj->getType() == ARROW
                                           || obj->getType() == THICCBOI))
        return false;
    if (bullet->shooterType() == SHIP && obj->getType() == SHIP)
        return false;
    int bullet_x = bullet->getLocation().first;
    int bullet_y = bullet->getLocation().second;
    int bullet_w = bullet->getDimension().first;
    int bullet_h = bullet->getDimension().second;
    return collide(obj->getLocation().first, obj->getLocation().second,
                   obj->getDimension().first, obj->getDimension().second,
                   bullet_x, bullet_y, bullet_w, bullet_h);
}

void aliensBulletsCollide(Alien_Maintainer* alienMtn, Bullet_Maintainer* bulletMtn){
    auto alienIter = alienMtn->begin();
    auto alienListEnd = alienMtn->end();
    while (alienIter != alienListEnd){
        auto bulletIter = bulletMtn->begin();
        auto bulletListEnd = bulletMtn->end();

        while (bulletIter != bulletListEnd){
            //*alienIter is a shared pointer to Alien
            //**alienIter is an Alien
            if (bulletObjCollide(&(**alienIter), &(**bulletIter)))
            {
                (*alienIter)->bulletHit();
                (*bulletIter)->setActivationState(false);
                break;
            }

            ++bulletIter;
        }
        ++alienIter;
    }
}

void Bullet_Maintainer::maintain(ShootableObject *ship,
                                 Alien_Maintainer *alienMaintainer){
    aliensBulletsCollide(alienMaintainer, this); //update if bullets hit aliens,
                                        //set active state of bullet according.
    auto bulletIter = bullet_list.begin();
    while (bulletIter != bullet_list.end()){
        bool erased = false;
        auto local_iter = bulletIter;
        bulletIter++;
        auto bulletPtr = *local_iter;
        bulletPtr->update();
        //check out of bound
        if (bulletPtr->getLocation().second < 0 ||
                            bulletPtr->getLocation().second > PRIM_DISPLAY_H ||
                            bulletPtr->getLocation().first < 0 ||
                            bulletPtr->getLocation().first > PRIM_DISPLAY_W)
        {
            bullet_list.erase(local_iter);
            erased = true;
        }
        //check bullet not actives because hit alien
        if (!bulletPtr->ifActive()){
            spark_list.push_back(BulletSpark(spark_array,
                                  bulletPtr->getLocation().first,
                                  bulletPtr->getLocation().second));
            if (!erased)
            {
                bullet_list.erase(local_iter);
                erased = true;
            }
        }
        //check if this bullet collide with a ship. Awful "if" statement.
        if (ship->getType() == SHIP && bulletPtr->shooterType() != SHIP
                && static_cast<Ship*>(ship)->getLives() > 0)
            if (collide(ship->getLocation().first,
                    ship->getLocation().second,
                    ship->getDimension().first,
                    ship->getDimension().second,
                    bulletPtr->getLocation().first,
                    bulletPtr->getLocation().second,
                    bulletPtr->getDimension().first,
                    bulletPtr->getDimension().second))
            {
                spark_list.push_back(BulletSpark(spark_array,
                                      bulletPtr->getLocation().first,
                                      bulletPtr->getLocation().second));
                if (!erased)
                {
                    bullet_list.erase(local_iter);
                    erased = true;
                }
                //tell the ship it got a hit if it is not in respawning time
                if (!static_cast<Ship*>(ship)->ifRespawning())
                    static_cast<Ship*>(ship)->gotShoot();
            }
        if (erased)
        {
            ++bulletDestroyed;
        }
    }
}

void Bullet_Maintainer::draw(){
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

void Bullet_Maintainer::add(ShootableObject *shooter){
    auto bulletPtr = bulletFactory->createBullet(bulletImages,shooter);
    if (bulletPtr){
        if (bulletPtr->speed_x == 0 && bulletPtr->speed_y == 0) //no moving bullet not allow
            return;
        bullet_list.push_back(std::shared_ptr<Bullet>(bulletPtr));
        ++bulletCreated;
    }
    else
        std::cout << "couldn't create bullet" << std::endl;
}

void Alien_Maintainer::maintain(Bullet_Maintainer *bulletMaintainer,
                                Score* score){
    auto alienIter = alienList.begin();
    while (alienIter != alienList.end()){
        auto local_iter = alienIter;
        alienIter++;
        auto alienPtr = *local_iter;
        alienPtr->update();
        if (alienPtr->getLocation().second > PRIM_DISPLAY_H )
        {
            alienList.erase(local_iter);
            continue;
        }
        if (alienPtr->readyToFire())
            bulletMaintainer->add(&(*alienPtr));
        if (!alienPtr->alive()) //because of bullet hit
        {
            explosion_list.push_back(Explosion(explosion_array,
                                    alienPtr->getLocation().first +
                                    alienPtr->getDimension().first/2,
                                     alienPtr->getLocation().second +
                                     alienPtr->getDimension().second/2));
            alienPtr->explodeSound();
            score->addScore(alienPtr->getScoreValue());
            alienList.erase(local_iter);
        }
    }
}

void Alien_Maintainer::draw(){
    auto iter = alienList.begin();
    while (iter != alienList.end()){
        auto local_iter = iter;
        iter++;
        auto ptr = *local_iter;
        ptr->draw();
    }
    auto explodeIter = explosion_list.begin();
    while (explodeIter != explosion_list.end())
    {
        auto local_iter = explodeIter;
        explodeIter++;
        if (local_iter->exploded())
            explosion_list.erase(local_iter);
        else local_iter->draw();
    }
}

Alien_Maintainer::Alien_Maintainer(Alien_Factory* factory, ALLEGRO_BITMAP* spritesheet){
    alienFactory = factory;
    sprite = spritesheet;
    ALLEGRO_BITMAP* bitmapPtr;
    std::string start_chars = "alien";
    Object_type aliens[] = {BUG, ARROW, THICCBOI};
    int x,y,w,h;
    for (int i = 1; i <= 3; ++i){ //config file list alien from 1
        std::string digits = std::to_string(i);
        x = std::atoi(al_get_config_value(config, "components",
                                          (start_chars + digits + "_x").c_str()));
        y = std::atoi(al_get_config_value(config, "components",
                                          (start_chars + digits + "_y").c_str()));
        w = std::atoi(al_get_config_value(config, "components",
                                          (start_chars + digits + "_w").c_str()));
        h = std::atoi(al_get_config_value(config, "components",
                                          (start_chars + digits + "_h").c_str()));
        bitmapPtr = sprite_grab(sprite, x,y,w,h);
        alienImages.push_back(Alien_image(bitmapPtr, aliens[i-1])); //i from 1
    }
//    //BUG:
//    x = std::atoi(al_get_config_value(config, "components", "bug_x"));
//    y = std::atoi(al_get_config_value(config, "components", "bug_y"));
//    w = std::atoi(al_get_config_value(config, "components", "bug_w"));
//    h = std::atoi(al_get_config_value(config, "components", "bug_h"));
//    bitmapPtr = sprite_grab(sprite, x,y,w,h);
//    alienImages.push_back(Alien_image(bitmapPtr, Object_type::BUG));
//    //ARROW:
//    x = std::atoi(al_get_config_value(config, "components", "arrow_x"));
//    y = std::atoi(al_get_config_value(config, "components", "arrow_y"));
//    w = std::atoi(al_get_config_value(config, "components", "arrow_w"));
//    h = std::atoi(al_get_config_value(config, "components", "arrow_h"));
//    bitmapPtr = sprite_grab(sprite, x,y,w,h);
//    alienImages.push_back(Alien_image(bitmapPtr, Object_type::ARROW));
//    //THICCBOI:
//    x = std::atoi(al_get_config_value(config, "components", "thiccboi_x"));
//    y = std::atoi(al_get_config_value(config, "components", "thiccboi_y"));
//    w = std::atoi(al_get_config_value(config, "components", "thiccboi_w"));
//    h = std::atoi(al_get_config_value(config, "components", "thiccboi_h"));
//    bitmapPtr = sprite_grab(sprite, x,y,w,h);
//    alienImages.push_back(Alien_image(bitmapPtr, Object_type::THICCBOI));
    //explostion frames effect, extracts from sprites
    start_chars = "explosion";
    for (int i = 0; i < 4; ++i){
        std::string digits = std::to_string(i);
        x = std::atoi(al_get_config_value(config, "components",
                                          (start_chars + digits + "_x").c_str()));
        y = std::atoi(al_get_config_value(config, "components",
                                          (start_chars + digits + "_y").c_str()));
        w = std::atoi(al_get_config_value(config, "components",
                                          (start_chars + digits + "_w").c_str()));
        h = std::atoi(al_get_config_value(config, "components",
                                          (start_chars + digits + "_h").c_str()));
        explosion_array[i] = sprite_grab(sprite, x,y,w,h);
    }

}

Bullet_Maintainer::Bullet_Maintainer(Bullet_factory* factory, ALLEGRO_BITMAP* spritesheet){
    bulletFactory = factory;
    sprite = spritesheet;
    ALLEGRO_BITMAP* bmp;
    std::string start_key_chars = "shipshot";
    int x, y, w, h;
    //SHIP:
    x = std::atoi(al_get_config_value(config, "components",
                                          (start_key_chars + "_x").c_str()));
    y = std::atoi(al_get_config_value(config, "components",
                                      (start_key_chars + "_y").c_str()));
    w = std::atoi(al_get_config_value(config, "components",
                                      (start_key_chars + "_w").c_str()));
    h = std::atoi(al_get_config_value(config, "components",
                                      (start_key_chars + "_h").c_str()));
        bmp = sprite_grab(spritesheet, x, y, w, h);
        bulletImages.push_back(Bullet_image(bmp, SHIP));
    //ALIEN:
    Object_type aliens[] = {BUG, ARROW, THICCBOI};
    start_key_chars = "alienshot";
    x = std::atoi(al_get_config_value(config, "components",
                                      (start_key_chars + "_x").c_str()));
    y = std::atoi(al_get_config_value(config, "components",
                                      (start_key_chars + "_y").c_str()));
    w = std::atoi(al_get_config_value(config, "components",
                                      (start_key_chars + "_w").c_str()));
    h = std::atoi(al_get_config_value(config, "components",
                                      (start_key_chars + "_h").c_str()));
    bmp = sprite_grab(spritesheet, x, y, w, h);
    for (int i = 0; i < 3; ++i){
        bulletImages.push_back(Bullet_image(bmp, aliens[i]));
    }

    //initialize images in spark_array:
    start_key_chars = "spark";
    for (int i = 0; i < 3; ++i){
        std::string digits = std::to_string(i);
        x = std::atoi(al_get_config_value(config, "components",
                                          (start_key_chars + digits + "_x").c_str()));
        y = std::atoi(al_get_config_value(config, "components",
                                          (start_key_chars + digits + "_y").c_str()));
        w = std::atoi(al_get_config_value(config, "components",
                                          (start_key_chars + digits + "_w").c_str()));
        h = std::atoi(al_get_config_value(config, "components",
                                          (start_key_chars + digits + "_h").c_str()));
        spark_array[i] = sprite_grab(sprite, x,y,w,h);
    }
}

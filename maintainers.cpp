#include "maintainers.h"
#include "ship.h"

bool bulletObjCollide(ShootableObject* obj, Bullet* bullet){
    if (bullet->shooterType() == ALIEN && (obj->getType() == BUG
                                           || obj->getType() == ARROW
                                           || obj->getType() == THICCBOI))
        return false;
    if (bullet->shooterType() == SHIP && obj->getType() == SHIP)
        return false;
    int bullet_x = std::get<0>(bullet->getBulletInfo());
    int bullet_y = std::get<1>(bullet->getBulletInfo());
    int bullet_w = std::get<2>(bullet->getBulletInfo());
    int bullet_h = std::get<3>(bullet->getBulletInfo());
    return collide(obj->getLocation().first, obj->getLocation().second,
                   obj->getDimension().first, obj->getDimension().second,
                   bullet_x, bullet_y, bullet_w, bullet_h);
}

void shotAndHit(Alien_Maintainer* alienMtn, Bullet_Maintainer* bulletMtn){
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
                (*alienIter)->endurance--;
                (*alienIter)->gotHit = true;
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
    auto iter = bullet_list.begin();
    while (iter != bullet_list.end()){
        auto local_iter = iter;
        iter++;
        auto ptr = *local_iter;
        ptr->update();
        if (ptr->getLocation().second < 0 ||
            ptr->getLocation().second > DISPLAY_H * SCALE ||
            ptr->getLocation().first < 0 ||
            ptr->getLocation().first > DISPLAY_W * SCALE)
            bullet_list.erase(local_iter);
        if (!ptr->ifActive()){
            spark_list.push_back(BulletSpark(spark_array,
                                              std::get<0>((*ptr).getBulletInfo()),
                                              std::get<1>((*ptr).getBulletInfo())));
            bullet_list.erase(local_iter);
        }
        if (ship->getType() == SHIP && ptr->shooterType() != SHIP
                && static_cast<Ship*>(ship)->getLives() > 0)
            if (collide(ship->getLocation().first,
                    ship->getLocation().second,
                    ship->getDimension().first,
                    ship->getDimension().second,
                    std::get<0>(ptr->getBulletInfo()),
                      std::get<1>(ptr->getBulletInfo()),
                      std::get<2>(ptr->getBulletInfo()),
                      std::get<3>(ptr->getBulletInfo())))
            {
                spark_list.push_back(BulletSpark(spark_array,
                                                  std::get<0>((*ptr).getBulletInfo()),
                                                  std::get<1>((*ptr).getBulletInfo())));
                bullet_list.erase(local_iter);
                if (!static_cast<Ship*>(ship)->ifRespawning())
                    static_cast<Ship*>(ship)->gotShoot();
            }
    }
    shotAndHit(alienMaintainer, this);
}

void Alien_Maintainer::maintain(Bullet_Maintainer *bulletMaintainer){
    auto iter = alienList.begin();
    while (iter != alienList.end()){
        auto local_iter = iter;
        iter++;
        auto ptr = *local_iter;
        ptr->update();
//            ptr->checkBulletHit(bulletMaintainer);
        if (ptr->getLocation().second > DISPLAY_H * SCALE )
            alienList.erase(local_iter);
        if (!ptr->alive())
        {
            explosion_list.push_back(Explosion(explosion_array,
                                    ptr->getLocation().first +
                                    ptr->getDimension().first/2,
                                     ptr->getLocation().second +
                                     ptr->getDimension().second/2));
            alienList.erase(local_iter);
        }
        if (ptr->readyToFire())
            bulletMaintainer->add(&(*ptr));
    }
}

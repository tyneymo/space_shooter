#ifndef SHOOTABLEOBJECT_H
#define SHOOTABLEOBJECT_H
#include "utilities.h"

class ShootableObject{
public:
    ShootableObject();

    std::pair<int,int> getLocation(){
        return std::make_pair(pos_x, pos_y);
    }

    std::pair<int, int> getDimension(){
        return std::make_pair(width, height);
    }

    //who call this function will also fire if it return true.
    virtual bool readyToFire(){
        if (fireNow){
            fireNow = false;
            fireCountdown = fireWait;
            return true;
        }
        return false;
    }

    Object_type getType(){
        return type;
    }
protected:
    Object_type type;
    int pos_x,pos_y,width,height;
    int fireCountdown=0;
    int fireWait;
    bool fireNow = false;
    bool gotHit = false;
    ALLEGRO_SAMPLE* shot_sample;
    ALLEGRO_SAMPLE* sample_explode[2];
};

#endif

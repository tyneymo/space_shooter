#ifndef UTILITIES_H
#define UTILITIES_H
#include <iostream>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

class ShootableObject;
//bool bulletObjCollide(ShootableObject* obj, Bullet* bullet);

enum Bullet_type {SHIP_BULLET1, SHIP_BULLET2, ALIEN_BULLET};
enum Object_type {SHIP, ALIEN, BUG, ARROW,THICCBOI};
const int DISPLAY_W = 320;
const int DISPLAY_H = 240;
const int SCALE = 2;

int between(int low, int high);

float between_f (float low, float high);

void must_init(bool test, const char* description);

bool collide(int obj1_x, int obj1_y, int obj1_w, int obj1_h,
               int obj2_x, int obj2_y, int obj2_w, int obj2_h);


class ShootableObject{
public:
    ShootableObject(){
        //audio file, hardcode :(
        shot_sample = al_load_sample("shot.flac");
        must_init(shot_sample, "shot sample");
        sample_explode[0] = al_load_sample("explode1.flac");
        must_init(sample_explode[0], "sample explode 1");
        sample_explode[1] = al_load_sample("explode2.flac");
        must_init(sample_explode[1], "sample explode 2");
    }

    std::pair<int,int> getLocation(){
        return std::make_pair(pos_x, pos_y);
    }

    std::pair<int, int> getDimension(){
        return std::make_pair(width, height);
    }

    //always fire when ask this function.
    virtual bool readyToFire(){
        if (fireNow){
            fireNow = false;
            fireCountdown = fireWait;
//            std::cout << "ready to fire" << std::endl;
            return true;
        }
//        std::cout << "alien count down to fire " << fireCountdown << std::endl;
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



//extract sprite from _sheet.
ALLEGRO_BITMAP* sprite_grab(ALLEGRO_BITMAP* bigmap,int x, int y, int w, int h);

//struct to save keyboard state
struct Keyboard{
    const int key_seen = 1;
    const int key_release = 2;
    unsigned char key[ALLEGRO_KEY_MAX]; //keep states of all keys on keyboard

    Keyboard(){
        //set all keys to 0.
        for (int i = 0; i < ALLEGRO_KEY_MAX; ++i)
            key[i] = 0;
    }

    void update(ALLEGRO_EVENT* event){
        switch (event->type){
            case ALLEGRO_EVENT_TIMER:
                //each frame, reset all bit of all key, keep only key that seen
                for (int i = 0; i < ALLEGRO_KEY_MAX; ++i)
                    { 
                        key[i] &= key_seen;
                    }
                break;

            //if a key down, update value at position according to the keycode
            case ALLEGRO_EVENT_KEY_DOWN:
                key[event->keyboard.keycode] = key_seen | key_release;
                break;
            
            //erase key_seen when key is up. keep only key_release. 
            //the timer will erase the key_release also. 
            case ALLEGRO_EVENT_KEY_UP:
                key[event->keyboard.keycode] &= key_release;
                break;
        }
    }
};
#endif // UTILITIES_H

#ifndef SHIP_H
#define SHIP_H
#include <iostream>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>

class Ship_factory;
class Ship{
    friend class Ship_factory;

public:
    ~Ship(){
        al_destroy_bitmap(ship_img);
    }

    void draw(){
        al_draw_bitmap(ship_img, pos_x, pos_y, 0);
    }

private:
    Ship(ALLEGRO_BITMAP* bitmap, int x, int y, int w, int h):
        pos_x(x), pos_y(y), width(w), height(h){}

    ALLEGRO_BITMAP* ship_img;
    int pos_x,pos_y,width,height;
};

#endif // SHIP_H

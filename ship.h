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
        pos_x(x), pos_y(y), width(w), height(h), ship_img(bitmap){}

    ALLEGRO_BITMAP* ship_img;
    int pos_x,pos_y,width,height;
};

class Ship_factory{
    Ship_factory(ALLEGRO_BITMAP* sheet): sprite(sheet){}
    Ship* createShip(int x, int y)    {
        ALLEGRO_BITMAP* ship_image = al_create_sub_bitmap(sprite, shipImage_x,
                                                          shipImage_y,
                                                          shipImage_w,
                                                          shipImage_h
                                                          );
        return new Ship(ship_image, x, y, shipImage_w, shipImage_h);
    }

private:
    ALLEGRO_BITMAP* sprite;
    int shipImage_x = 0, shipImage_y = 0, shipImage_w = 12, shipImage_h = 13;
};

#endif // SHIP_H

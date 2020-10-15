#ifndef UTILITIES_H
#define UTILITIES_H
#include <iostream>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_image.h>

void must_init(bool test, const char* description){
    if (test) return;

    std::cout << "can not initialize " << description << std::endl;
    exit(1);
}

//extract sprite from _sheet.
ALLEGRO_BITMAP* sprite_grab(ALLEGRO_BITMAP* bigmap,int x, int y, int w, int h){
    ALLEGRO_BITMAP* sprite = al_create_sub_bitmap(bigmap, x, y, w, h);
    must_init(sprite, "sprite grab");
    return sprite;
}
#endif // UTILITIES_H

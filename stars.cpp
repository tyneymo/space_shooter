#include "stars.h"

extern ALLEGRO_CONFIG* config;
extern int PRIM_DISPLAY_W, PRIM_DISPLAY_H;
extern int EFFECTIVE_DISPLAY_DIAG;
extern int EFFECTIVE_DISPLAY_HEIGHT;
extern int EFFECTIVE_DRAWING_DIMENSION;
extern float FRAMERATE;
extern float FRAMERATEMULTIPLIER;

int Star::elementsNumber = 0;

Star::Star(ALLEGRO_BITMAP* bmp){
    starImg = bmp;
    ratio = between_f(0,1.0);
    effectCounter = between(0, 60*FRAMERATEMULTIPLIER);
}

void Star::draw(){
    float drawRatio;
    int cutPoint = 30*FRAMERATEMULTIPLIER;
    //give stars effect of slow blinking
    if ((effectCounter / cutPoint) % 2)
        drawRatio = (float)(cutPoint - (effectCounter++ % cutPoint)) / cutPoint;
    else drawRatio = (float)(effectCounter++ % cutPoint) / cutPoint;
    int w = al_get_bitmap_width(starImg);
    int h = al_get_bitmap_height(starImg);
    al_draw_scaled_rotated_bitmap(starImg,w/2,h/2,pos_x,
                                  pos_y, drawRatio*ratio,
                                  drawRatio*ratio, 0, 0);
    //when a star get dimmed, it relocate itself.
    if (!(effectCounter % (2*cutPoint))){
        relocate();
    }
}

void Star::relocate(){
    int element_w = PRIM_DISPLAY_W/elementsNumber;
    int element_h = PRIM_DISPLAY_H/elementsNumber;
    int ran = between(0,elementsNumber);
    pos_x = between (ran * element_w, (ran+1) * element_w);
    ran = between(0,elementsNumber);
    pos_y = between (ran * element_h, (ran+1) * element_h);
}

AllStars::AllStars(){
    //create a star bitmap model then feed it to star's constructor
    starModel = al_create_bitmap(PRIM_DISPLAY_H/40,
                                 PRIM_DISPLAY_H/40);
    must_init(starModel, "create model for stars");
    ALLEGRO_BITMAP* saveDisp = al_get_target_bitmap();
    al_set_target_bitmap(starModel);
    al_clear_to_color(al_map_rgba(0,0,0,0));
    int rectSide = al_get_bitmap_height(starModel);
    ALLEGRO_COLOR colorWhite = al_map_rgb_f(0.5,0.5,0.5);
    al_draw_filled_triangle(rectSide/2, 0, rectSide/3, rectSide/2, rectSide*2/3,
                            rectSide/2,colorWhite);
    al_draw_filled_triangle(0, rectSide/2, rectSide/2, rectSide/3, rectSide/2,
                            rectSide*2/3,colorWhite);
    al_draw_filled_triangle(rectSide/2, rectSide, rectSide/3, rectSide/2,
                            rectSide*2/3, rectSide/2, colorWhite);
    al_draw_filled_triangle(rectSide, rectSide/2, rectSide/2, rectSide/3,
                            rectSide/2, rectSide*2/3, colorWhite);
    //restore drawing to original
    al_set_target_bitmap(saveDisp);
    stars.reserve(elementsNumber*elementsNumber);
    int element_w = PRIM_DISPLAY_W/elementsNumber;
    int element_h = PRIM_DISPLAY_H/elementsNumber;
    int sz = elementsNumber*elementsNumber;
    //create stars and distribute stars all over screen
    Star::elementsNumber = elementsNumber;
    for (int i = 0; i < sz; ++i){
        stars.emplace_back(starModel);
        int ran = between(0,elementsNumber);
        stars[i].pos_x = between (ran * element_w, (ran+1) * element_w);
        ran = between(0,elementsNumber);
        stars[i].pos_y = between (ran * element_h, (ran+1) * element_h);
    }
}

//only called when resize.
void AllStars::update(){
    int element_w = PRIM_DISPLAY_W/elementsNumber;
    int element_h = PRIM_DISPLAY_H/elementsNumber;
    int sz = stars.size();
    for (int i = 0; i < sz; ++i){
        int ran = between(0,elementsNumber);
        stars[i].pos_x = between (ran * element_w, (ran+1) * element_w);
        ran = between(0,elementsNumber);
        stars[i].pos_y = between (ran * element_h, (ran+1) * element_h);
    }
}

void AllStars::draw(){
    int sz = stars.size();
    for (int i = 0; i < sz; ++i)
        stars[i].draw();
}

ALLEGRO_CONFIG* loadConfig(){
    ALLEGRO_CONFIG* config = al_load_config_file("ssconfig.ini");
    if (addConfig(config))
        config = al_load_config_file("ssconfig.ini");
    return config;
}

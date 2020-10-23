#include "utilities.h"
#include "ship.h"
#include <cstdlib>
#include <string>
#include <cmath>
#include <allegro5/allegro_primitives.h>

extern ALLEGRO_CONFIG* config;
extern int PRIM_DISPLAY_W, PRIM_DISPLAY_H;
extern int EFFECTIVE_DISPLAY_DIAG;
extern int EFFECTIVE_DISPLAY_HEIGHT;
extern float FRAMERATE;
extern float FRAMERATEMULTIPLIER;


int between(int low, int high){
    return low + (rand() % (high - low));
}


float between_f (float low, float high){
    return low + (((float)rand() / (float)RAND_MAX)) * (high - low);
}

bool collide(int obj1_x, int obj1_y, int obj1_w, int obj1_h,
               int obj2_x, int obj2_y, int obj2_w, int obj2_h){
    if ((obj1_x + obj1_w < obj2_x) || (obj1_x > obj2_x + obj2_w) ||
        (obj1_y + obj1_h < obj2_y) || (obj1_y > obj2_y + obj2_h))
        return false;
    return true;
}

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

void drawPlayerInformation(ALLEGRO_BITMAP* lifeImg ,ALLEGRO_FONT* font, Score& score,
                           bool askForEnding, bool pausing,Ship& ship1, Ship& ship2){
    al_draw_textf(font, al_map_rgb_f(1,1,1), 3, 3, 0,
                  "%ld", score.getScore()); //draw score at x=3 y=3
    int ship1LivesLeft = ship1.getLives();
    int ship2LivesLeft = ship2.getLives();
    int lifeImgWid = al_get_bitmap_width(lifeImg);
    if (askForEnding){
        al_draw_text(font, al_map_rgb_f(1,1,1),PRIM_DISPLAY_W/2, PRIM_DISPLAY_H/2 + 2*al_get_font_line_height(font),
                     ALLEGRO_ALIGN_CENTER, "press Y to end game");
        al_draw_text(font, al_map_rgb_f(1,1,1),PRIM_DISPLAY_W/2, PRIM_DISPLAY_H/2 + 3*al_get_font_line_height(font),
                     ALLEGRO_ALIGN_CENTER, "press ESC again to continue");
    }
    if (!(ship1LivesLeft || ship2LivesLeft))
    {
        al_draw_text(font, al_map_rgb_f(1,1,1),PRIM_DISPLAY_W/2, PRIM_DISPLAY_H/2,
                     ALLEGRO_ALIGN_CENTER, "G A M E  O V E R");
        return;
    }
    if (pausing){
        al_draw_text(font, al_map_rgb_f(1,1,1),PRIM_DISPLAY_W/2, PRIM_DISPLAY_H/2,
                     ALLEGRO_ALIGN_CENTER, "P A U S E");
    }
    for (int i = 0; i < ship2LivesLeft; ++i){
        al_draw_bitmap(lifeImg, 3 + lifeImgWid*i, 12, 0 );
    }
    for (int i = 0; i < ship1LivesLeft; ++i){
        al_draw_bitmap(lifeImg, PRIM_DISPLAY_W - (3 + lifeImgWid*(i+1)), 12, 0 );
    }
}

ShootableObject::ShootableObject(){
    //audio file, hardcode :(
    shot_sample = al_load_sample(al_get_config_value(config,
                                    "files_name", "shot"));
    must_init(shot_sample, "shot sample, please check audio file name for shot!");
    sample_explode[0] = al_load_sample(al_get_config_value(config,
                                    "files_name", "explode1"));
    must_init(sample_explode[0],
            "sample explode 1, please check audio file name for explode 1!");
    sample_explode[1] = al_load_sample(al_get_config_value(config,
                                    "files_name", "explode2"));
    must_init(sample_explode[1],
            "sample explode 2, please check audio file name for explode 2!");
}

Star::Star(ALLEGRO_BITMAP* bmp){
    ALLEGRO_BITMAP* saveDisp = al_get_target_bitmap();
    int rectSide = al_get_bitmap_height(bmp);
    std::cout << rectSide << std::endl;
    float floatRand = between_f(0,1.0);
    int newSide = rectSide*floatRand;
    std::cout << newSide << std::endl;
    ALLEGRO_BITMAP* dest = al_create_bitmap(newSide, newSide);
    must_init(dest , "create bitmap for star");
    al_set_target_bitmap(dest);
    al_draw_scaled_bitmap(bmp,0,0,rectSide,rectSide, 0,0, newSide,newSide,0);
    starImg = dest;
    al_set_target_bitmap(saveDisp);
    std::cout << "a star was born" << std::endl;
    effectCounter = between(0, 30*FRAMERATEMULTIPLIER);
}

void Star::draw(){
    float drawRatio;
    int cutPoint = 40*FRAMERATEMULTIPLIER;
    //give stars effect of slow blinking
    drawRatio = (float)(effectCounter++ % cutPoint) / cutPoint;
    int bmpWid  = al_get_bitmap_width(starImg);
    int bmpHei = al_get_bitmap_height(starImg);
    int drawWid = drawRatio * bmpWid;
    int drawHei = drawRatio * bmpHei;
    al_draw_scaled_bitmap(starImg, 0,0,bmpWid, bmpHei, pos_x, pos_y, drawWid,
                          drawHei, 0);
    //when a star get highest size, it relocate itself.
    if (!(effectCounter % cutPoint)){
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
    ALLEGRO_BITMAP* bmpModel = al_create_bitmap(PRIM_DISPLAY_H/20,
                                                PRIM_DISPLAY_H/40);
    must_init(bmpModel, "create model for stars");
    ALLEGRO_BITMAP* saveDisp = al_get_target_bitmap();
    al_set_target_bitmap(bmpModel);
    al_clear_to_color(al_map_rgb(0,0,0));
    int rectSide = al_get_bitmap_height(bmpModel);
    ALLEGRO_COLOR colorWhite = al_map_rgb_f(0.5,0.5,0.5);
    al_draw_filled_triangle(rectSide/2, 0, rectSide/3, rectSide/2, rectSide*2/3,
                            rectSide/2,colorWhite);
    al_draw_filled_triangle(0, rectSide/2, rectSide/2, rectSide/3, rectSide/2,
                            rectSide*2/3,colorWhite);
    al_draw_filled_triangle(rectSide/2, rectSide, rectSide/3, rectSide/2,
                            rectSide*2/3, rectSide/2, colorWhite);
    al_draw_filled_triangle(rectSide, rectSide/2, rectSide/3, rectSide/2,
                            rectSide/2, rectSide*2/3, colorWhite);
    //restore drawing to original
    al_set_target_bitmap(saveDisp);
    stars.reserve(elementsNumber*elementsNumber);
    int element_w = PRIM_DISPLAY_W/elementsNumber;
    int element_h = PRIM_DISPLAY_H/elementsNumber;
    int sz = elementsNumber*elementsNumber;
    //create stars and distribute stars all over screen
    for (int i = 0; i < sz; ++i){
        stars.emplace_back(bmpModel);
        stars[i].elementsNumber = elementsNumber;
        int ran = between(0,elementsNumber);
        stars[i].pos_x = between (ran * element_w, (ran+1) * element_w);
        ran = between(0,elementsNumber);
        stars[i].pos_y = between (ran * element_h, (ran+1) * element_h);
    }
    al_destroy_bitmap(bmpModel);
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

void setDisplayValues(ALLEGRO_CONFIG* config){
    PRIM_DISPLAY_W = std::atoi(al_get_config_value(config, "display",
                                                   "PRIM_DISPLAY_WIDTH"));
    PRIM_DISPLAY_H = std::atoi(al_get_config_value(config, "display",
                                                   "PRIM_DISPLAY_HEIGHT"));
    FRAMERATE = std::atoi(al_get_config_value(config, "display",
                                              "FRAMERATE"));

    //for some code calculate speed base on dividend, if the display dimensions
    //and framerate config is not appropriate (too small display, too high frame
    //rate), things might not move. So here the code check for bad config.
    if (PRIM_DISPLAY_H < 240)
    {
        PRIM_DISPLAY_H = 400;
        PRIM_DISPLAY_W = 600;
        FRAMERATE = 30;
        std::cout << "Too small display config" << std::endl;
    }

    FRAMERATEMULTIPLIER = FRAMERATE/30.0;
    int diag = std::sqrt(PRIM_DISPLAY_W*PRIM_DISPLAY_W +
                            PRIM_DISPLAY_H*PRIM_DISPLAY_H);
    if (diag > 1.33 * PRIM_DISPLAY_H) //just to limit effective diag
        diag = 1.33 * PRIM_DISPLAY_H;
    EFFECTIVE_DISPLAY_DIAG = diag / FRAMERATEMULTIPLIER;
    if (EFFECTIVE_DISPLAY_DIAG < 320){
        FRAMERATE = 30;
        FRAMERATEMULTIPLIER = 1.0;
        EFFECTIVE_DISPLAY_DIAG = diag;
        std::cout << "Too high frame rate and/or too small display config"
                  << std::endl;
    }
    EFFECTIVE_DISPLAY_HEIGHT = PRIM_DISPLAY_H / FRAMERATEMULTIPLIER;
}

bool addConfig(ALLEGRO_CONFIG* config){
    if (config) //nothing to do,
        return false;
    config = al_create_config();
    std::string section = "files_name";
    al_add_config_section(config, section.c_str());
    al_set_config_value(config, section.c_str(), "sprite_sheet",
                        "spritesheet.png");
    al_set_config_value(config, section.c_str(), "shot", "shot.flac");
    al_set_config_value(config, section.c_str(), "explode1", "explode1.flac");
    al_set_config_value(config, section.c_str(), "explode2", "explode2.flac");
    section = "display";
    al_add_config_section(config, section.c_str());
    al_set_config_value(config, section.c_str(), "PRIM_DISPLAY_WIDTH", "960");
    al_set_config_value(config, section.c_str(), "PRIM_DISPLAY_HEIGHT", "720");
    al_set_config_value(config, section.c_str(), "FRAMERATE", "60");
    section = "components";
    al_add_config_section(config, section.c_str());
    al_set_config_value(config, section.c_str(), "ship_x", "0");
    al_set_config_value(config, section.c_str(), "ship_y", "0");
    al_set_config_value(config, section.c_str(), "ship_w", "12");
    al_set_config_value(config, section.c_str(), "ship_h", "13");
    al_set_config_value(config, section.c_str(), "shipshot_x", "13");
    al_set_config_value(config, section.c_str(), "shipshot_y", "0");
    al_set_config_value(config, section.c_str(), "shipshot_w", "2");
    al_set_config_value(config, section.c_str(), "shipshot_h", "9");
    al_set_config_value(config, section.c_str(), "life_x", "0");
    al_set_config_value(config, section.c_str(), "life_y", "14");
    al_set_config_value(config, section.c_str(), "life_w", "6");
    al_set_config_value(config, section.c_str(), "life_h", "6");
    al_set_config_value(config, section.c_str(), "alien0_x", "19");
    al_set_config_value(config, section.c_str(), "alien0_y", "0");
    al_set_config_value(config, section.c_str(), "alien0_w", "14");
    al_set_config_value(config, section.c_str(), "alien0_h", "9");
    al_set_config_value(config, section.c_str(), "alien1_x", "19");
    al_set_config_value(config, section.c_str(), "alien1_y", "10");
    al_set_config_value(config, section.c_str(), "alien1_w", "13");
    al_set_config_value(config, section.c_str(), "alien1_h", "10");
    al_set_config_value(config, section.c_str(), "alien2_x", "0");
    al_set_config_value(config, section.c_str(), "alien2_y", "21");
    al_set_config_value(config, section.c_str(), "alien2_w", "45");
    al_set_config_value(config, section.c_str(), "alien2_h", "27");
    al_set_config_value(config, section.c_str(), "alienshot_x", "13");
    al_set_config_value(config, section.c_str(), "alienshot_y", "10");
    al_set_config_value(config, section.c_str(), "alienshot_w", "4");
    al_set_config_value(config, section.c_str(), "alienshot_h", "4");
    al_set_config_value(config, section.c_str(), "explosion0_x", "33");
    al_set_config_value(config, section.c_str(), "explosion0_y", "10");
    al_set_config_value(config, section.c_str(), "explosion0_w", "9");
    al_set_config_value(config, section.c_str(), "explosion0_h", "9");
    al_set_config_value(config, section.c_str(), "explosion1_x", "43");
    al_set_config_value(config, section.c_str(), "explosion1_y", "9");
    al_set_config_value(config, section.c_str(), "explosion1_w", "11");
    al_set_config_value(config, section.c_str(), "explosion1_h", "11");
    al_set_config_value(config, section.c_str(), "explosion2_x", "46");
    al_set_config_value(config, section.c_str(), "explosion2_y", "21");
    al_set_config_value(config, section.c_str(), "explosion2_w", "17");
    al_set_config_value(config, section.c_str(), "explosion2_h", "18");
    al_set_config_value(config, section.c_str(), "explosion3_x", "46");
    al_set_config_value(config, section.c_str(), "explosion3_y", "40");
    al_set_config_value(config, section.c_str(), "explosion3_w", "17");
    al_set_config_value(config, section.c_str(), "explosion3_h", "17");
    al_set_config_value(config, section.c_str(), "spark0_x", "34");
    al_set_config_value(config, section.c_str(), "spark0_y", "0");
    al_set_config_value(config, section.c_str(), "spark0_w", "10");
    al_set_config_value(config, section.c_str(), "spark0_h", "8");
    al_set_config_value(config, section.c_str(), "spark1_x", "45");
    al_set_config_value(config, section.c_str(), "spark1_y", "0");
    al_set_config_value(config, section.c_str(), "spark1_w", "7");
    al_set_config_value(config, section.c_str(), "spark1_h", "8");
    al_set_config_value(config, section.c_str(), "spark2_x", "54");
    al_set_config_value(config, section.c_str(), "spark2_y", "0");
    al_set_config_value(config, section.c_str(), "spark2_w", "9");
    al_set_config_value(config, section.c_str(), "spark2_h", "8");
    if (al_save_config_file("ssconfig.ini", config))
    {
        std::cout << "Created a default config file, name \"ssconfig.ini\"" <<
                                                                    std::endl;
        al_destroy_config(config);
        return true;
    }
    else std::cout << "Cannot create config file" << std::endl;
        return false;
}

ALLEGRO_BITMAP* getLifeBmp(ALLEGRO_BITMAP* spritesheet){
    int x = atoi(al_get_config_value(config, "components", "life_x"));
    int y = atoi(al_get_config_value(config, "components", "life_y"));
    int w = atoi(al_get_config_value(config, "components", "life_w"));
    int h = atoi(al_get_config_value(config, "components", "life_h"));
    ALLEGRO_BITMAP* tempBmp = sprite_grab(spritesheet, x,y,w,h);
    ALLEGRO_BITMAP* saveDisplay = al_get_target_bitmap();
    int displayWidth = al_get_bitmap_width(saveDisplay);
    int displayHeight = al_get_bitmap_height(saveDisplay);
    ALLEGRO_BITMAP* lifeBmp = al_create_bitmap(displayWidth/53, displayHeight/40);
    al_set_target_bitmap(lifeBmp);
    al_clear_to_color(al_map_rgb(0,0,0));
    al_draw_scaled_bitmap(tempBmp,0,0,w,h,0,0,displayWidth/53, displayHeight/40,0);
    al_set_target_bitmap(saveDisplay);
    al_destroy_bitmap(tempBmp);
    return lifeBmp;
}

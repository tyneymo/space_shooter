#include "utilities.h"
#include "ship.h"
#include <cstdlib>
#include <string>

extern ALLEGRO_CONFIG* config;
extern int PRIM_DISPLAY_W, PRIM_DISPLAY_H;

int between(int low, int high){
    return low + (rand() % (high - low));
}


float between_f (float low, float high){
    return low + ((float)rand() / (float)RAND_MAX) / (high - low);
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

void drawPlayerInformation(ALLEGRO_BITMAP* lifeImg ,ALLEGRO_FONT* font,Score* score,
                           Ship* ship1, Ship* ship2){
    al_draw_textf(font, al_map_rgb_f(1,1,1), 3, 3, 0,
                  "%ld", score->getScore()); //draw score at x=3 y=3
    int ship1LivesLeft = ship1->getLives();
    int ship2LivesLeft = ship2->getLives();
    int lifeImgWid = al_get_bitmap_width(lifeImg);
    if (!(ship1LivesLeft || ship2LivesLeft))
    {
        al_draw_text(font, al_map_rgb_f(1,1,1),PRIM_DISPLAY_W/2, PRIM_DISPLAY_H/2,
                     ALLEGRO_ALIGN_CENTER, "G A M E  O V E R");
        return;
    }
    for (int i = 0; i < ship2LivesLeft; ++i){
        al_draw_bitmap(lifeImg, 3 + lifeImgWid*i, 12, 0 );
    }
    for (int i = 0; i < ship1LivesLeft; ++i){
        al_draw_bitmap(lifeImg, PRIM_DISPLAY_W - (9 + lifeImgWid*i), 12, 0 );
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

ALLEGRO_CONFIG* loadConfig(){
    ALLEGRO_CONFIG* config = al_load_config_file("ssconfig.ini");
    addConfig(config);
    return config;
}

void setDisplayValues(ALLEGRO_CONFIG* config){
    PRIM_DISPLAY_W = std::atoi(al_get_config_value(config, "display",
                                                   "PRIM_DISPLAY_WIDTH"));
    PRIM_DISPLAY_H = std::atoi(al_get_config_value(config, "display",
                                                   "PRIM_DISPLAY_HEIGHT"));
}

void addConfig(ALLEGRO_CONFIG* config){
    if (config) //nothing to do,
        return;
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
    al_set_config_value(config, section.c_str(), "alien1_x", "19");
    al_set_config_value(config, section.c_str(), "alien1_y", "0");
    al_set_config_value(config, section.c_str(), "alien1_w", "14");
    al_set_config_value(config, section.c_str(), "alien1_h", "9");
    al_set_config_value(config, section.c_str(), "alien2_x", "19");
    al_set_config_value(config, section.c_str(), "alien2_y", "10");
    al_set_config_value(config, section.c_str(), "alien2_w", "13");
    al_set_config_value(config, section.c_str(), "alien2_h", "10");
    al_set_config_value(config, section.c_str(), "alien3_x", "0");
    al_set_config_value(config, section.c_str(), "alien3_y", "21");
    al_set_config_value(config, section.c_str(), "alien3_w", "45");
    al_set_config_value(config, section.c_str(), "alien3_h", "27");
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
    al_save_config_file("ssconfig.ini", config);
}

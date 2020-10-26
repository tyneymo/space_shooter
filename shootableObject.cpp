#include "shootableObject.h"
#include <iostream>

extern ALLEGRO_CONFIG* config;

ShootableObject::ShootableObject(){
    //constructor go to source file to get extern config
    //then redirect to generateSample which will initialize samples once only.
    generateSamples(config);

}

void ShootableObject::generateSamples(ALLEGRO_CONFIG* config){
    static ALLEGRO_SAMPLE* shot;
    static std::array<ALLEGRO_SAMPLE*,2> explode;
    static bool inited = false;
    if (!inited)
    {
        shot = al_load_sample(al_get_config_value(config,
                                                         "files_name", "shot"));
        must_init(shot, "shot sample, please check audio file name for shot!");
        explode[0] = al_load_sample(al_get_config_value(config,"files_name",
                                                        "explode1"));
        must_init(explode[0],
                "sample explode 1, please check audio file name for explode 1!");
        explode[1] = al_load_sample(al_get_config_value(config,"files_name",
                                                        "explode2"));
        must_init(explode[1],
                "sample explode 2, please check audio file name for explode 2!");
        inited = true;
    }
    shot_sample = shot;
    sample_explode = explode;
}

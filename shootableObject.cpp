#include "shootableObject.h"

extern ALLEGRO_CONFIG* config;

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

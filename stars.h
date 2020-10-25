#ifndef STARS_H
#define STARS_H

#include "utilities.h"
class Star{
public:
    friend class AllStars;
    Star(ALLEGRO_BITMAP*);
    void draw();

private:
    void relocate();
    float ratio;
    ALLEGRO_BITMAP* starImg;
    int pos_x, pos_y;
    int effectCounter;
    static int elementsNumber; //number of drawing elements per side on screen
};

class AllStars{
public:
    AllStars();
    ~AllStars(){al_destroy_bitmap(starModel);}
    void update();
    void draw();

private:
    ALLEGRO_BITMAP* starModel;
    std::vector<Star> stars;
    int elementsNumber = 15;
};
#endif // STARS_H

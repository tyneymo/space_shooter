#include "ship.h"
#include "utilities.h"
#include "maintainers.h"

ALLEGRO_CONFIG* config;
int PRIM_DISPLAY_W, PRIM_DISPLAY_H;


int main(int argc, char** argv)
{
    must_init(al_init(), "allegro init");
    must_init(al_install_keyboard(), "keyboard");
    must_init(al_init_image_addon(), "allegro image addon");
    must_init(al_install_audio(), "allegro audio");
    must_init(al_init_acodec_addon(), "allegro audio codec");
    must_init(al_reserve_samples(16), "reserve samples");
    must_init(al_init_font_addon(), "font addon init");
    config = loadConfig();
    setDisplayValues(config);

    ALLEGRO_TIMER* timer = al_create_timer(1.0/30.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "event queue");

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);

    ALLEGRO_DISPLAY* disp = al_create_display(PRIM_DISPLAY_W,
                                              PRIM_DISPLAY_H);
    ALLEGRO_BITMAP* buffer = al_create_bitmap(PRIM_DISPLAY_W, PRIM_DISPLAY_H);
    must_init(disp, "display init");
    must_init(buffer, "display buffer");
    ALLEGRO_FONT* font = al_create_builtin_font();
    must_init(font, "font init");

    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));

    ALLEGRO_BITMAP* spritesheet = al_load_bitmap(al_get_config_value(config,
                                                 "files_name", "sprite_sheet"));
    must_init(spritesheet, "init sprite, please check image file name!");
    Ship_factory aShipFactory(spritesheet);

    ALLEGRO_BITMAP* lifeBmp = sprite_grab(spritesheet, 0, 14, 6, 6);

    std::shared_ptr<Ship> ship_one(aShipFactory.createShip(2*PRIM_DISPLAY_W /3 ,
                                                           4*PRIM_DISPLAY_H /5));
    std::shared_ptr<Ship> ship_two(aShipFactory.createShip(PRIM_DISPLAY_W /3,
                                                           4*PRIM_DISPLAY_H /5));

    ship_two->set_control(ALLEGRO_KEY_W, ALLEGRO_KEY_S, 
                          ALLEGRO_KEY_A, ALLEGRO_KEY_D, ALLEGRO_KEY_F);


    bool redraw = true;
    bool done = false;

    ALLEGRO_EVENT event;
    al_start_timer(timer);
    Keyboard keyboard;

    Bullet_factory bulletFactory;
    Bullet_Maintainer bulletMaintainer (&bulletFactory, spritesheet);
    Alien_Factory alienFactory;
    Alien_Maintainer alienMaintainer(&alienFactory, spritesheet);
    long frameCounter = 0;
    Score score;
    while(1){
        al_wait_for_event(queue, &event);

        switch(event.type){
            case ALLEGRO_EVENT_TIMER:
                //update position and fire_ready state
                ++frameCounter;
                ship_one->update(&keyboard);
                ship_two->update(&keyboard);
                if (ship_one->readyToFire())
                    bulletMaintainer.add(&(*ship_one));
                if (ship_two->readyToFire())
                    bulletMaintainer.add(&(*ship_two));
                if (!(frameCounter % 40))
                    alienMaintainer.add();
                alienMaintainer.maintain(&bulletMaintainer, &score);
                bulletMaintainer.maintain(&(*ship_one), &alienMaintainer);
                bulletMaintainer.maintain(&(*ship_two), &alienMaintainer);
                redraw = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
                    done = true;
                break;

            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        keyboard.update(&event);

        if (done) break;

        if (redraw && al_is_event_queue_empty(queue))
        {
            if (buffer != al_get_target_bitmap())
                al_set_target_bitmap(buffer);
            al_clear_to_color(al_map_rgb(0,0,0));
            ship_one->draw();
            ship_two->draw();
            alienMaintainer.draw();
            bulletMaintainer.draw();
            al_set_target_backbuffer(disp);
            al_clear_to_color(al_map_rgb(0,0,0));
            al_draw_scaled_bitmap(buffer, 0, 0, PRIM_DISPLAY_W, PRIM_DISPLAY_H,
                                  0, 0, PRIM_DISPLAY_W, PRIM_DISPLAY_H, 0);
            drawPlayerInformation(lifeBmp,font, &score, &(*ship_one), &(*ship_two));
            al_flip_display();
            al_set_target_bitmap(buffer);
            redraw = false;
        }

    }

    al_destroy_timer(timer);
    al_destroy_event_queue(queue);
    al_destroy_display(disp);
    al_destroy_bitmap(buffer);
    al_destroy_bitmap(spritesheet);
    al_destroy_bitmap(lifeBmp);
    al_destroy_font(font);
    al_destroy_config(config);
    return 0;
}

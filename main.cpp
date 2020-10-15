#include <iostream>
#include <ship.h>
#include <utilities.h>

using namespace std;

int main()
{
    must_init(al_init(), "allegro init");
    must_init(al_install_keyboard(), "keyboard");
    must_init(al_init_image_addon(), "allegro image addon");

    ALLEGRO_TIMER* timer = al_create_timer(1.0/30.0);
    must_init(timer, "timer");

    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    must_init(queue, "event queue");

    al_set_new_display_option(ALLEGRO_SAMPLE_BUFFERS, 1, ALLEGRO_SUGGEST);
    al_set_new_display_option(ALLEGRO_SAMPLES, 8, ALLEGRO_SUGGEST);
    al_set_new_bitmap_flags(ALLEGRO_MIN_LINEAR | ALLEGRO_MAG_LINEAR);
    al_set_blender(ALLEGRO_ADD, ALLEGRO_ONE, ALLEGRO_ONE);

    ALLEGRO_DISPLAY* disp = al_create_display(320 * 3, 240 * 3);
    must_init(disp, "display init");

    al_register_event_source(queue, al_get_timer_event_source(timer));
    al_register_event_source(queue, al_get_keyboard_event_source());
    al_register_event_source(queue, al_get_display_event_source(disp));

    ALLEGRO_EVENT event;

    al_start_timer(timer);

    ALLEGRO_BITMAP* spritesheet = al_load_bitmap("spritesheet.png");
    Ship_factory aShipFactory(spritesheet);

    Ship* ship_one = aShipFactory.createShip(320, 240);

    bool redraw = true;
    bool done = false;

    while(1){
        al_wait_for_event(queue, &event);

        switch(event.type){
            case ALLEGRO_EVENT_TIMER:
                ship_one->draw();
                redraw = true;
                break;

            case ALLEGRO_EVENT_KEY_DOWN:
            case ALLEGRO_EVENT_DISPLAY_CLOSE:
                done = true;
                break;
        }

        if (done) break;

        if (redraw && al_is_event_queue_empty(queue))
        {
            al_flip_display();
            redraw = false;
        }

    }
    return 0;
}

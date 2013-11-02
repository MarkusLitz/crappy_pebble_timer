#include "display.h"

#define TOGGLE(x) x = !x

bool flash_background = true;
bool light_enabled    = false;

Window window;

TextLayer timer_m;
TextLayer timer_s;

// Define some needed values
#define SCREEN_WIDTH        144
#define SCREEN_HEIGHT       168

#define TIME_IMAGE_WIDTH    70
#define TIME_IMAGE_HEIGHT   70

#define DATE_IMAGE_WIDTH    20
#define DATE_IMAGE_HEIGHT   20

#define SECOND_IMAGE_WIDTH  10
#define SECOND_IMAGE_HEIGHT 10

#define DAY_IMAGE_WIDTH     20
#define DAY_IMAGE_HEIGHT    10

#define MARGIN              1
#define TIME_SLOT_SPACE     2
#define DATE_PART_SPACE     4
	

void update_display_with_time(int time_left) {
    static char time_text_m[] = "00:00:00";
	static char time_text_s[] = "00:00:00";
    PblTm pebble_time_left = {
        .tm_sec = time_left%60,
        .tm_min = time_left/60
    };

    string_format_time(time_text_m, sizeof(time_text_m), "%M", &pebble_time_left);
	string_format_time(time_text_s, sizeof(time_text_s), "%S", &pebble_time_left);

    text_layer_set_text(&timer_m,time_text_m);
    text_layer_set_text(&timer_s,time_text_s);
}

void alert() {
    vibes_short_pulse();
    TOGGLE(light_enabled);
    // XXX Need to figure out how to do this in a way that doesn't cause the
    // light to just stay on.
    //light_enable(light_enabled);
}

void warning() {
    int foreground = (flash_background ? GColorBlack : GColorWhite);
    int background = (flash_background ? GColorWhite : GColorBlack);
    text_layer_set_text_color(&timer_m, foreground);
	text_layer_set_text_color(&timer_s, foreground);
    window_set_background_color(&window, background);
    TOGGLE(flash_background);
}

Window* get_window() {
    return &window;
}

void initialize_display() {
  window_init(&window, "Timer");
  window_stack_push(&window, true);
  window_set_background_color(&window, GColorBlack);

  text_layer_init(&timer_m, GRect(0, 18, 110, 50));
  text_layer_set_text_color(&timer_m, GColorWhite);
  text_layer_set_background_color(&timer_m, GColorClear);
  text_layer_set_font(&timer_m, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(&timer_m, GTextAlignmentRight);    
  layer_add_child(&window.layer, &timer_m.layer);
	
  text_layer_init(&timer_s, GRect(0, 70, 110, 50));
  text_layer_set_text_color(&timer_s, GColorWhite);
  text_layer_set_background_color(&timer_s, GColorClear);
  text_layer_set_font(&timer_s, fonts_get_system_font(FONT_KEY_BITHAM_42_LIGHT));
  text_layer_set_text_alignment(&timer_s, GTextAlignmentRight);
  layer_add_child(&window.layer, &timer_s.layer);


  text_layer_set_text(&timer_m, "Starting...");

}

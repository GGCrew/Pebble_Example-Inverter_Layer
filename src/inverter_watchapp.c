#include <pebble.h>

// Define global variables
Window *window;
TextLayer *text_time_layer;
char time_text[] = "00:00";

// Function to convert the time value into a string and update the time layer
void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  // We're converting into military time because it requires less code
  strftime(time_text, sizeof(time_text), "%R", tick_time);
  text_layer_set_text(text_time_layer, time_text);
}

// Function to initialize our program -- keeping our main() block clean!
void init(void) {
  // Declare your variables!
  Layer *window_layer;

  // Create the main window
  window = window_create();
  window_stack_push(window, true);
  window_set_background_color(window, GColorBlack);

  // Grab this layer once now, instead of repeatedly calling the function later
  window_layer = window_get_root_layer(window);

  // FONT_KEY_ROBOTO_BOLD_SUBSET_49 is defined in pebble_fonts.h
  // GRect(0, 78, 144, 32) is rougly in the center of the Pebble screen
  text_time_layer = text_layer_create(GRect(0, 54, 144, 64));
  text_layer_set_text_color(text_time_layer, GColorWhite);
  text_layer_set_background_color(text_time_layer, GColorClear);
  text_layer_set_font(text_time_layer, fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49));
  text_layer_set_text_alignment(text_time_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_time_layer));

  // Hook a function onto the timer
  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
}

// Function to clean up our program -- keeping our main() block clean!
// This is so small that it *could* fit into our main() block,
//  but having a dedicated function is a good habit for when 
//  more complex programs need to deinitialize multiple objects.
// It also pairs nicely with the init() function.
void deinit(void) {
  tick_timer_service_unsubscribe();
}


// The main program loop
int main(void) {
  init();
  app_event_loop();
  deinit();
}


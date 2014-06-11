#include <pebble.h>

// Define global variables
Window *window;
TextLayer *text_time_layer;
InverterLayer *inverter_layer;
PropertyAnimation *inverter_animation;
char time_text[] = "00:00";

// Function to convert the time value into a string and update the time layer
void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) {
  // We're converting into military time because it requires less code
  strftime(time_text, sizeof(time_text), "%R", tick_time);
  text_layer_set_text(text_time_layer, time_text);

  // Start the animation
  animation_schedule(&inverter_animation->animation);	
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
  // GRect(0, 54, 144, 64) is roughly in the center of the Pebble screen
  text_time_layer = text_layer_create(GRect(0, 54, 144, 64));
  text_layer_set_text_color(text_time_layer, GColorWhite);
  text_layer_set_background_color(text_time_layer, GColorClear);
  text_layer_set_font(text_time_layer, fonts_get_system_font(FONT_KEY_ROBOTO_BOLD_SUBSET_49));
  text_layer_set_text_alignment(text_time_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_time_layer));

  // Set inverter layer size to full screen
  inverter_layer = inverter_layer_create(GRect(0, 0, 144, 168));
  layer_add_child(window_layer, inverter_layer_get_layer(inverter_layer));

  // Create the animation for the layer and set some animation properties
  // The animation will start at the top of the screen (0, 0, 144, 168), and
  // scroll down to the bottom (0, 167, 144, 168).
  // Using AnimationCurveLinear so the animation speed is constant.
  // Duration is measured in milliseconds, so 1000 = 1 second and 60000 = 1 minute
  inverter_animation = property_animation_create_layer_frame(inverter_layer_get_layer(inverter_layer), &GRect(0, 0, 144, 168), &GRect(0, 167, 144, 168));
  animation_set_curve(&inverter_animation->animation, AnimationCurveLinear);
  animation_set_duration(&inverter_animation->animation, 60000);

  // Hook a function onto the timer
  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
}

// Function to clean up our program -- keeping our main() block clean!
// This is so small that it *could* fit into our main() block,
//  but having a dedicated deinit() function is a good habit for when 
//  more complex programs need to deinitialize multiple objects.
// It also pairs nicely with the init() function.
void deinit(void) {
  tick_timer_service_unsubscribe();
  property_animation_destroy(inverter_animation);
  inverter_layer_destroy(inverter_layer);
  text_layer_destroy(text_time_layer);
}


// The main program loop
int main(void) {
  init();
  app_event_loop();
  deinit();
}


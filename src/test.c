// /*
//   Source: https://github.com/pebble-examples/strider-watchface/blob/master/src/Strider.c
// */

// #include <pebble.h>

// static Window *s_window;
// static Layer *s_window_layer;
// static TextLayer *s_step_layer;

// static char s_current_time_buffer[8];
// static int s_step_count = 0;

// // Is step data available?
// bool step_data_is_available() {
//   return HealthServiceAccessibilityMaskAvailable &
//     health_service_metric_accessible(HealthMetricStepCount,
//       time_start_of_today(), time(NULL));
// }

// // Todays current step count
// static void get_step_count() {
// //   s_step_count = (int)health_service_sum_today(HealthMetricStepCount);
    
//     //Hardcoded value for testing on cloud pebble, no way to fake data... yet.
//      s_step_count = 100; 
// }

// /*
//  * Writes the step count to the screen
//  */
// static void display_step_count() {
//   /* The string/char-buffer to hold the string representation of int.
//    * Source: http://stackoverflow.com/questions/20435527/how-to-convert-int-to-string-with-pebble-sdk-in-c
//    */
//   static char buf[] = "00000000000";
//   snprintf(buf, sizeof(buf), "%d", s_step_count);

//   text_layer_set_text(s_step_layer, buf);
  
//   // Request a redraw
//   layer_mark_dirty(s_window_layer);
// }

// static void health_handler(HealthEventType event, void *context) {
//   //if we have a step event then let's updated the layers by marking them as dirty
//   if(event != HealthEventSleepUpdate) {
//     //retrieve new values
//     get_step_count();
    
//     //Update the displayed text
//     display_step_count();
    
//   }
// }


// static void window_load(Window *window) {
//   // Create a layer to hold the current step count  
//   s_step_layer = text_layer_create(GRect(0, 0, 144, 40));
//   text_layer_set_text_alignment(s_step_layer, GTextAlignmentCenter);
//   display_step_count();
//   layer_add_child(s_window_layer, text_layer_get_layer(s_step_layer));

//   // Subscribe to health events if we can, otherwise just a basic watchface
//   if(step_data_is_available()) {
//     health_service_events_subscribe(health_handler, NULL);
//   }
// }

// static void window_unload(Window *window) {
//   //Destroy all of the layers
//   layer_destroy(text_layer_get_layer(s_step_layer));
// }

// void init() {

//   s_window = window_create();
//   s_window_layer = window_get_root_layer(s_window);
//   window_set_background_color(s_window, GColorBlack);

//   //Handlers for window events
//   window_set_window_handlers(s_window, (WindowHandlers) {
//     .load = window_load,
//     .unload = window_unload
//   });

//   //pushes the window to the stack
//   window_stack_push(s_window, true);
// }

// void deinit() {
//     //Nothing to do here for strider.... Could do things like saving data to persistent storage
// }

// /*
//  * App Entrypoint
//  */
// int main() {
//   //Setup
//   init();
    
//   //Blocking until the windowstack is empty (app is ready to exit)
//   app_event_loop();
    
//   //Removal
//   deinit();
// }
#include <pebble.h>

static Window *s_main_window;
static Layer *window_layer;
static TextLayer *s_output_layer;

//
static AppSync s_sync;
//

static int s_vmc_count = 0;

/*callback
static void in_received_handler(DictionaryIterator *iter, void *context)
{
 
}*/

static void refresh_vmc_count() {
  APP_LOG(APP_LOG_LEVEL_INFO, "refresh");
  
  /* The string/char-buffer to hold the string representation of int.
   * Source: http://stackoverflow.com/questions/20435527/how-to-convert-int-to-string-with-pebble-sdk-in-c
   */
  static char buf[] = "00000000000";
  snprintf(buf, sizeof(buf), "%d", s_vmc_count);

  text_layer_set_text(s_output_layer, buf);
  
  // Request a redraw
  layer_mark_dirty(window_layer);
}

static void get_vmc_count() {
  // Create an array to store data
  const uint32_t max_records = 60;
  HealthMinuteData *minute_data = (HealthMinuteData*)
                                malloc(max_records * sizeof(HealthMinuteData));

  // Make a timestamp for curr time and 1 min before 
  time_t end = time(NULL);
  time_t start = end - 3; //SECONDS_PER_MINUTE;

  // Obtain the minute-by-minute records
  uint32_t num_records = health_service_get_minute_history(minute_data, 
                                                    max_records, &start, &end);
  APP_LOG(APP_LOG_LEVEL_INFO, "num_records: %d", (int)num_records);

  // Print the vmc for each minute
  for(uint32_t i = 0; i < num_records; i++) {
    APP_LOG(APP_LOG_LEVEL_INFO, "Item %d vmc: %d", (int)i, 
            (int)minute_data[i].vmc);

    s_vmc_count = (int)minute_data[i].vmc;
    refresh_vmc_count();

  }
  // Free the array
  free(minute_data);
}

static void health_handler(HealthEventType event, void *context) {
  if(event == HealthEventMovementUpdate) {
    APP_LOG(APP_LOG_LEVEL_INFO, "New health movement event");
  }
  
  get_vmc_count();  
}


/*
static void button_click() {
  // Declare the dictionary's iterator
  DictionaryIterator *out_iter;

  // Prepare the outbox buffer for this message
  AppMessageResult result = app_message_outbox_begin(&out_iter);
  if(result == APP_MSG_OK) {
    // Add an item to ask for weather data
    int value = 0;
    
    //
    dict_write_int(out_iter, AppKeyRequestData, &value, sizeof(int), true);

    // Send this message
    result = app_message_outbox_send();
    if(result != APP_MSG_OK) {
      APP_LOG(APP_LOG_LEVEL_ERROR, "Error sending the outbox: %d", (int)result);
    }
  }
  else {
    // The outbox cannot be used right now
    APP_LOG(APP_LOG_LEVEL_ERROR, "Error preparing the outbox: %d", (int)result);
  }
  
  // Deallocate dictionary
  free(out_iter);
}
*/

static void main_window_load(Window *window) {
  window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  const int text_height = 20;
  const GEdgeInsets text_insets = GEdgeInsets((bounds.size.h - text_height) / 2, 0);

  s_output_layer = text_layer_create(grect_inset(bounds, text_insets));
  text_layer_set_text_alignment(s_output_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_output_layer));
  refresh_vmc_count();
  
  health_service_events_subscribe(health_handler, NULL);

}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_output_layer);
}

static void init(void) {  
  s_main_window = window_create();
  
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  
  /*Register AppMessage events
  app_message_register_inbox_received(in_received_handler);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());    //Largest possible input and output buffer sizes
  */
  
  window_stack_push(s_main_window, true);
  
  /*
  app_message_open(64, 64);
  */
}

static void deinit(void) {
  window_destroy(s_main_window);
  
  /*
  app_sync_deinit(&s_sync);
  */
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}


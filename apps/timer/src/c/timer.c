#include <pebble.h>

static Window *s_window;
static TextLayer *s_text_layer;

static int  time_value = 20;
static char time_display[10];

static void set_time() {
  snprintf(time_display, 10, "%d", time_value);
  text_layer_set_text(s_text_layer, time_display);
}

static void stop() {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "STOP. time_value: %d", time_value);
  tick_timer_service_unsubscribe();
}

static void tick(struct tm *tick_time, TimeUnits units_changed) {
	light_enable_interaction();
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TICK. time_value: %d", time_value);
	if(time_value <= 1) {
		time_value = 0;
		set_time();
		stop();
		vibes_short_pulse();
	} else {
		time_value -= 1;
		set_time();
	}
}

static void start() {
  vibes_short_pulse();
  APP_LOG(APP_LOG_LEVEL_DEBUG, "START. time_value: %d", time_value);
  tick_timer_service_subscribe(SECOND_UNIT, tick);
}

static void prv_select_click_handler(ClickRecognizerRef recognizer, void *context) {
  start();
}

static void prv_up_click_handler(ClickRecognizerRef recognizer, void *context) {
  stop();
  time_value += 1;
  set_time();
}

static void prv_up_long_click_handler(ClickRecognizerRef recognizer, void *context) {
  stop();
  time_value += 10;
  set_time();
}

static void prv_down_click_handler(ClickRecognizerRef recognizer, void *context) {
  stop();
  time_value -= 1;
  set_time();
}

static void prv_down_long_click_handler(ClickRecognizerRef recognizer, void *context) {
  stop();
  time_value -= 10;
  set_time();
}

static void prv_click_config_provider(void *context) {
	window_single_click_subscribe(BUTTON_ID_SELECT, prv_select_click_handler);
	window_single_click_subscribe(BUTTON_ID_UP, prv_up_click_handler);
	window_single_click_subscribe(BUTTON_ID_DOWN, prv_down_click_handler);

	window_long_click_subscribe(BUTTON_ID_UP, 0, prv_up_long_click_handler, NULL);
	window_long_click_subscribe(BUTTON_ID_DOWN, 0, prv_down_long_click_handler, NULL);
}


static void prv_window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_text_layer = text_layer_create(GRect(0, 55, bounds.size.w, bounds.size.h));

  text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_BITHAM_42_MEDIUM_NUMBERS));
  text_layer_set_text_alignment(s_text_layer, GTextAlignmentCenter);

  set_time();

  layer_add_child(window_layer, text_layer_get_layer(s_text_layer));
}

static void prv_window_unload(Window *window) {
  text_layer_destroy(s_text_layer);
}

static void prv_init(void) {
  s_window = window_create();
  window_set_click_config_provider(s_window, prv_click_config_provider);
  window_set_window_handlers(s_window, (WindowHandlers) {
    .load = prv_window_load,
    .unload = prv_window_unload,
  });
  const bool animated = true;
  window_stack_push(s_window, animated);
}

static void prv_deinit(void) {
  window_destroy(s_window);
}

int main(void) {
  prv_init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", s_window);

  app_event_loop();
  prv_deinit();
}

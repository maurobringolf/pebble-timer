#include <pebble.h>

static const uint32_t STORAGE_KEY = 0;

static Window *s_window;
static TextLayer *s_text_layer;

static int time_value = 0;
static char time_display[10];

static bool RUNNING = false;

static void set_time() {
	persist_write_int(STORAGE_KEY, time_value);
	snprintf(time_display, 10, "%d", time_value);
	text_layer_set_text(s_text_layer, time_display);
}

static void stop() {
	if(!RUNNING) return;
	APP_LOG(APP_LOG_LEVEL_DEBUG, "STOP. time_value: %d", time_value);
	tick_timer_service_unsubscribe();
	RUNNING = false;
}

static void tick(struct tm *tick_time, TimeUnits units_changed) {
	APP_LOG(APP_LOG_LEVEL_DEBUG, "TICK. time_value: %d", time_value);
	if (RUNNING) {
		time_value += 1;
	}
	set_time();
	RUNNING = true;
}

static void start() {
	if (RUNNING) return;
	light_enable_interaction();
	APP_LOG(APP_LOG_LEVEL_DEBUG, "START. time_value: %d", time_value);
	tick_timer_service_subscribe(MINUTE_UNIT, tick);
}

static void prv_select_click_handler(ClickRecognizerRef recognizer, void *context) {
	start();
}

static void prv_up_click_handler(ClickRecognizerRef recognizer, void *context) {
	stop();
}

static void prv_click_config_provider(void *context) {
	window_single_click_subscribe(BUTTON_ID_SELECT, prv_select_click_handler);
	window_single_click_subscribe(BUTTON_ID_UP, prv_up_click_handler);
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

	time_value = persist_read_int(STORAGE_KEY);

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

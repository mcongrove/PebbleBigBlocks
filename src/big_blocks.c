/**
 * This code is provided under the Apache 2.0 license.
 * Please read the LICENSE.md file for more information
 * 
 * Copyright (c) 2013 Matthew Congrove (http://github.com/mcongrove)
 */
#include <pebble.h>

static char THEME[6] = "dark";
static int TIME_DISPLAY = 0;

Window *window;
TextLayer *label_layer_hour;
TextLayer *label_layer_minute;
InverterLayer *inverter_layer;
char hourText[] = "00";
char minuteText[] = "00";

typedef struct {
  Layer *block_layer;
} BlockData;

static BlockData blocks[12];

enum {
	KEY_THEME,
	KEY_TIME_DISPLAY
};

static int get_id_by_position(int x, int y) {
	switch(y) {
		case 0:
			switch(x) {
				case 0:
					return 10;
					break;
				case 48:
					return 11;
					break;
				case 96:
					return 0;
					break;
				default:
					return 0;
			}
			break;
		case 33:
			switch(x) {
				case 0:
					return 9;
					break;
				case 96:
					return 1;
					break;
				default:
					return 0;
			}
			break;
		case 67:
			switch(x) {
				case 0:
					return 8;
					break;
				case 96:
					return 2;
					break;
				default:
					return 0;
			}
			break;
		case 101:
			switch(x) {
				case 0:
					return 7;
					break;
				case 96:
					return 3;
					break;
				default:
					return 0;
			}
			break;
		case 135:
			switch(x) {
				case 0:
					return 6;
					break;
				case 48:
					return 5;
					break;
				case 96:
					return 4;
					break;
				default:
					return 0;
			}
			break;
		default:
			return 0;
	}
}

static GRect get_position_by_id(int id) {
	switch(id) {
		case 0:
			return GRect(96, 0, 48, 33);
			break;
		case 1:
			return GRect(96, 33, 48, 34);
			break;
		case 2:
			return GRect(96, 67, 48, 34);
			break;
		case 3:
			return GRect(96, 101, 48, 34);
			break;
		case 4:
			return GRect(96, 135, 48, 33);
			break;
		case 5:
			return GRect(48, 135, 48, 33);
			break;
		case 6:
			return GRect(0, 135, 48, 33);
			break;
		case 7:
			return GRect(0, 101, 48, 34);
			break;
		case 8:
			return GRect(0, 67, 48, 34);
			break;
		case 9:
			return GRect(0, 33, 48, 34);
			break;
		case 10:
			return GRect(0, 0, 48, 33);
			break;
		case 11:
			return GRect(48, 0, 48, 33);
			break;
		default:
			return GRect(0, 0, 48, 34);
	}
}

static void mark_all_dirty() {
	for (int i = 0, x = 12; i < x; i++) {
		BlockData *block_data = &blocks[i];
		layer_mark_dirty(block_data->block_layer);
	}
}

static void set_theme() {
	if (persist_exists(KEY_THEME)) {
		persist_read_string(KEY_THEME, THEME, 7);
	}
	
	APP_LOG(APP_LOG_LEVEL_INFO, "SELECTED THEME: %s", THEME);
	
	bool hide = strcmp(THEME, "light") == 0 ? true : false;
	
	layer_set_hidden(inverter_layer_get_layer(inverter_layer), hide);
}

static void set_time_display() {
	if (persist_exists(KEY_TIME_DISPLAY)) {
		TIME_DISPLAY = persist_read_int(KEY_TIME_DISPLAY);
	}
	
	APP_LOG(APP_LOG_LEVEL_INFO, "SELECTED TIME DISPLAY: %d", TIME_DISPLAY);
	
	switch(TIME_DISPLAY) {
		case 1:
			layer_set_frame(text_layer_get_layer(label_layer_hour), GRect(48, 65, 48, 77));
			layer_set_hidden(text_layer_get_layer(label_layer_hour), false);
			layer_set_hidden(text_layer_get_layer(label_layer_minute), true);
			break;
		case 2:
			layer_set_hidden(text_layer_get_layer(label_layer_hour), true);
			layer_set_frame(text_layer_get_layer(label_layer_minute), GRect(48, 65, 48, 77));
			layer_set_hidden(text_layer_get_layer(label_layer_minute), false);
			break;
		case 3:
			layer_set_frame(text_layer_get_layer(label_layer_hour), GRect(48, 40, 48, 51));
			layer_set_hidden(text_layer_get_layer(label_layer_hour), false);
			layer_set_frame(text_layer_get_layer(label_layer_minute), GRect(48, 90, 48, 51));
			layer_set_hidden(text_layer_get_layer(label_layer_minute), false);
			break;
		default:
			layer_set_hidden(text_layer_get_layer(label_layer_hour), true);
			layer_set_hidden(text_layer_get_layer(label_layer_minute), true);
	}
}

static void in_received_handler(DictionaryIterator *iter, void *context) {
	Tuple *theme_tuple = dict_find(iter, KEY_THEME);
	Tuple *time_display_tuple = dict_find(iter, KEY_TIME_DISPLAY);
	
	if (theme_tuple) {
		APP_LOG(APP_LOG_LEVEL_INFO, "SETTING THEME: %s", theme_tuple->value->cstring);

		persist_write_string(KEY_THEME, theme_tuple->value->cstring);
		strncpy(THEME, theme_tuple->value->cstring, 6);
		
		set_theme();
	}
	
	if (time_display_tuple) {
		int time_display = time_display_tuple->value->data[0];
		
		APP_LOG(APP_LOG_LEVEL_INFO, "SETTING TIME DISPLAY: %d", time_display);
		
		persist_write_int(KEY_TIME_DISPLAY, time_display);
		TIME_DISPLAY = time_display;
		
		set_time_display();
	}
}

static void in_dropped_handler(AppMessageResult reason, void *context) {
	
}

static void handle_hour_tick(struct tm *tick_time, TimeUnits units_changed) {
	mark_all_dirty();
	
	time_t t = time(NULL);
	struct tm *now = localtime(&t);
	
	char *time_format_hour;
	char *time_format_minute;
	
	if(clock_is_24h_style()) {
		time_format_hour = "%H";
	} else {
		time_format_hour = "%I";
	}
	
	time_format_minute = "%M";
	
	strftime(hourText, sizeof(hourText), time_format_hour, now);
	strftime(minuteText, sizeof(minuteText), time_format_minute, now);
	
	text_layer_set_text(label_layer_hour, hourText);
	text_layer_set_text(label_layer_minute, minuteText);
}

static void block_layer_update_callback(Layer *layer, GContext* ctx) {
	time_t now = time(NULL);
	struct tm *t = localtime(&now);
	
	GRect bounds = layer_get_bounds(layer);
	GRect frame = layer_get_frame(layer);
	
	int id = get_id_by_position(frame.origin.x, frame.origin.y);
	int hour = t->tm_hour > 12 ? t->tm_hour - 12 : t->tm_hour;
	
	graphics_context_set_fill_color(ctx, GColorWhite);
	
	if (id == 11 || (t->tm_hour != 0 && t->tm_hour != 12 && id < hour)) {
		graphics_context_set_fill_color(ctx, GColorBlack);
	}
	
	graphics_fill_rect(ctx, bounds, 0, GCornerNone);
}

static void create_block_layers() {
	Layer *window_layer = window_get_root_layer(window);
	
	for (int i = 0, x = 12; i < x; i++) {
		BlockData *block_data = &blocks[i];

		block_data->block_layer = layer_create(get_position_by_id(i));
		layer_set_update_proc(block_data->block_layer, block_layer_update_callback);
		layer_add_child(window_layer, block_data->block_layer);
	}
}

static void init() {
	app_message_register_inbox_received(in_received_handler);
	app_message_register_inbox_dropped(in_dropped_handler);
	app_message_open(64, 0);
	
	window = window_create();
	window_set_background_color(window, GColorWhite);
	window_stack_push(window, true);
	
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_bounds(window_layer);
	
	create_block_layers();
	
	// Create the hour layer
	label_layer_hour = text_layer_create(GRect(48, 40, 48, 51));
	text_layer_set_text_color(label_layer_hour, GColorBlack);
	text_layer_set_background_color(label_layer_hour, GColorClear);
	text_layer_set_text_alignment(label_layer_hour, GTextAlignmentCenter);
	text_layer_set_font(label_layer_hour, fonts_get_system_font(FONT_KEY_GOTHIC_28));
	layer_add_child(window_layer, text_layer_get_layer(label_layer_hour));
	
	// Create the minute layer
	label_layer_minute = text_layer_create(GRect(48, 90, 48, 51));
	text_layer_set_text_color(label_layer_minute, GColorBlack);
	text_layer_set_background_color(label_layer_minute, GColorClear);
	text_layer_set_text_alignment(label_layer_minute, GTextAlignmentCenter);
	text_layer_set_font(label_layer_minute, fonts_get_system_font(FONT_KEY_GOTHIC_28));
	layer_add_child(window_layer, text_layer_get_layer(label_layer_minute));
	
	// Create the inverter layer
	inverter_layer = inverter_layer_create(bounds);
	layer_add_child(window_layer, inverter_layer_get_layer(inverter_layer));
	
	tick_timer_service_subscribe(HOUR_UNIT, handle_hour_tick);
	
	set_theme();
	set_time_display();
}

static void deinit() {
	window_destroy(window);
	text_layer_destroy(label_layer_hour);
	text_layer_destroy(label_layer_minute);
	inverter_layer_destroy(inverter_layer);
	
	for (int i = 0, x = 12; i < x; i++) {
		BlockData *block_data = &blocks[i];
		layer_destroy(block_data->block_layer);
	}
	
	tick_timer_service_unsubscribe();
	app_message_deregister_callbacks();
}

int main(void) {
	init();
	app_event_loop();
	deinit();
}
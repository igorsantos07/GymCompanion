#include <pebble.h>

Window *window;
TextLayer *text_layer;

static void begin() {
	window       = window_create();
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_frame(window_layer);
	text_layer   = text_layer_create((GRect){ .origin = { 0, 30 }, .size = bounds.size });
	text_layer_set_text(text_layer, clock_is_24h_style()? "Mode:\n24h" : "Mode:\n12h");
	text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
	layer_add_child(window_layer, text_layer_get_layer(text_layer));
	window_stack_push(window, true);
}

static void end() {
	text_layer_destroy(text_layer);
	window_destroy(window);
}

int main(void) {
	begin(); app_event_loop(); end();
}

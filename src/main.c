#include <pebble.h>

#define MAX_WORKOUTS 7
#define MAX_EXERCISES 20

typedef struct {
	char *name;
	int sets, reps, weight;
} Exercise;

Window *window;
TextLayer *exercise_layer, *titles_layer, *numbers_layer;

static void setWorkouts(int key) {
	Exercise ex;
	ex.name = "";
	snprintf(ex.name, 10, "Teste #%02d", key);
	ex.sets = key;
	ex.reps = 10+key;
	ex.weight = 30*key;
	persist_write_data(key, &ex, sizeof(ex));
}

static Exercise getWorkouts(int key) {
	Exercise read_ex;

	if (persist_exists(key))
		persist_read_data(key, &read_ex, sizeof(read_ex));

	return read_ex;
}

static void begin(void) {
	window              = window_create();
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_frame(window_layer);

	exercise_layer  = text_layer_create((GRect){ .origin = { 0, 0 }, .size = { .w = bounds.size.w, .h = bounds.size.h/2 } });
	text_layer_set_font(exercise_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
	text_layer_set_text_alignment(exercise_layer, GTextAlignmentCenter);

	titles_layer  = text_layer_create((GRect){ .origin = { 0,                 (bounds.size.h/2) }, .size = { bounds.size.w/2, bounds.size.h/2 } });
	numbers_layer = text_layer_create((GRect){ .origin = { (bounds.size.w/2)+5, (bounds.size.h/2) }, .size = { (bounds.size.w/2)-5, bounds.size.h/2 } });
	text_layer_set_font(titles_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
	text_layer_set_text_alignment(titles_layer, GTextAlignmentRight);
	text_layer_set_font(numbers_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
	text_layer_set_text_alignment(numbers_layer, GTextAlignmentLeft);

	// setWorkouts(1);
	setWorkouts(2);
	Exercise my_ex = getWorkouts(2);

	char numbers[15];
	// snprintf(numbers, 15, "%dx\n%d\n%dkg", 4, 10, 35);
	// text_layer_set_text(exercise_layer, "Blabla Whiskas Sache");
	// text_layer_set_text(titles_layer, "Sets:\nReps:\nWeight:");
	snprintf(numbers, 15, "%dx\n%d\n%dkg", my_ex.sets, my_ex.reps, my_ex.weight);
	text_layer_set_text(exercise_layer, my_ex.name);
	text_layer_set_text(titles_layer, "Sets:\nReps:\nWeight:");
	text_layer_set_text(numbers_layer, numbers);

	layer_add_child(window_layer, text_layer_get_layer(exercise_layer));
	layer_add_child(window_layer, text_layer_get_layer(titles_layer));
	layer_add_child(window_layer, text_layer_get_layer(numbers_layer));
	window_stack_push(window, true);
}

static void end(void) {
	text_layer_destroy(exercise_layer);
	text_layer_destroy(titles_layer);
	text_layer_destroy(numbers_layer);
	window_destroy(window);
}

int main(void) {
	begin(); app_event_loop(); end();
}



	// int e = 1;
	// struct Exercise exercise;
	//this works
	// struct Exercise ex = { 4, 10, (12*e) };
	// exercise = ex;

	//this also works
	// struct Exercise exercises[] = {
	// 	{ 4, 10, (12*e++) },
	// 	{ 4, 10, (12*e++) },
	// 	{ 4, 10, (12*e++) },
	// 	{ 4, 10, (12*e++) }
	// };
	// exercise = exercises[3];

	//this don't work
	// struct Exercise exercises[MAX_EXERCISES];
	// exercises[e] = { 4, 10, (11*e++) };
	// exercises[e] = { 4, 10, (11*e++) };
	// exercises[e] = { 4, 10, (11*e++) };
	// exercise = exercises[2];

	//this don't work
	// struct Exercise exercises[MAX_EXERCISES];
	// exercises++ = { 4, 10, (11*e++) };
	// exercises++ = { 4, 10, (11*e++) };
	// exercises++ = { 4, 10, (11*e++) };
	// exercise = exercises[2];

	//this will create garbage
	// struct Exercise exercises[MAX_EXERCISES];
	// exercises[e].sets = 4;
	// exercises[e].reps = 10;
	// exercises[e].weight = 11*e;
	// exercise = exercises[e];

	//this works
	// struct Exercise ex;
	// ex.sets = 4;
	// ex.reps = 10;
	// ex.weight = 11*e;
	// exercise = ex;

	//this creates more garbage
	// struct Exercise exercises[MAX_EXERCISES];
	// exercises[0].sets = 4;
	// exercises[0].sets = 10;
	// exercises[0].weight = 35;
	// exercise = exercises[0];

	//this creates empty stuff
	// struct Exercise exercises[MAX_EXERCISES];
	// struct Exercise ex;
	// ex.sets = 4;
	// ex.sets = 10;
	// ex.weight = 35;
	// exercises[0] = ex;
	// exercise = exercises[0];
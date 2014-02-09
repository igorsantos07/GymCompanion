#include <pebble.h>

typedef struct {
	char *name;
	int sets, reps, weight;
} Exercise;

typedef struct {
	char letter;
	Exercise *exercises;
} Workout;

int total_workouts = 0;

Window *window;
TextLayer *exercise_layer, *titles_layer, *numbers_layer;

static char itoc(int number) {
	return number + 64;
}

static Exercise newExercise(char *name, int sets, int reps, int weight) {
	Exercise ex = { .name = name, .sets = sets, .reps = reps, .weight = weight };
	return ex;
}

static Workout newWorkout(int number, Exercise *exercises) {
	Workout w = { .letter = itoc(number), .exercises = exercises };
	++total_workouts;
	return w;
}

static void setWorkout(int i) {
	Exercise exs[3] = {
		newExercise("Abdominal", 4, 20, 0),
		newExercise("Supino", 4, 12, 15),
		newExercise("Tríceps testa", 4, 12, 35)
	};
	Workout workout = newWorkout(i, exs);

	persist_write_data(i, &workout, sizeof(workout));
}

static Workout getWorkout(int key) {
	Workout read_data;

	if (persist_exists(key))
		persist_read_data(key, &read_data, sizeof(read_data));

	return read_data;
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

	setWorkout(1);
	Workout my_workout = getWorkout(1);
	Exercise my_ex = my_workout.exercises[2];

	char numbers[15], title[100];
	snprintf(numbers, sizeof(numbers), "%dx\n%d\n%dkg", my_ex.sets, my_ex.reps, my_ex.weight);
	snprintf(title, sizeof(title), "[%c] %s", my_workout.letter, my_ex.name);
	text_layer_set_text(exercise_layer, title);
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
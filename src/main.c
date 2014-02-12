#include <pebble.h>

#define MAX_WORKOUTS 7

typedef struct {
	char *name;
	int   sets,
	      reps,
	      weight;
} Exercise;

typedef struct {
	char      letter;
	Exercise *exercises;
} Workout;

int icon_ids[] = { RESOURCE_ID_ICON_WORKOUT_A, RESOURCE_ID_ICON_WORKOUT_B,
	RESOURCE_ID_ICON_WORKOUT_C, RESOURCE_ID_ICON_WORKOUT_D, RESOURCE_ID_ICON_WORKOUT_E,
	RESOURCE_ID_ICON_WORKOUT_F, RESOURCE_ID_ICON_WORKOUT_G
};

int total_workouts = 0;
Workout workouts[MAX_WORKOUTS];

SimpleMenuLayer  *workout_menu_layer;
SimpleMenuItem    workout_menu[MAX_WORKOUTS];
SimpleMenuSection workout_sections[1];

Window    *window;
TextLayer *exercise_layer,
          *titles_layer,
          *numbers_layer;

static char itoc(int number) { return number + 64; }
static int ctoi(char letter) { return letter - 64; } 

static Exercise newExercise(char *name, int sets, int reps, int weight) {
	Exercise ex = { .name = name, .sets = sets, .reps = reps, .weight = weight };
	return ex;
}

/*
 * TODO: should stop creating workouts when total_workouts = MAX_WORKOUTS and log that instead
 * TODO: this function should be syncronized with the workouts array!
 */
static Workout newWorkout(int number, Exercise *exercises) {
	Workout w = { .letter = itoc(number), .exercises = exercises };
	++total_workouts;
	return w;
}

static void setWorkout(int key) {
	Exercise exs[3] = {
		newExercise("Abdominal", 4, 20, 0),
		newExercise("Supino", 4, 12, 15),
		newExercise("Tríceps testa", 4, 12, 35)
	};
	Workout workout = newWorkout(key, exs);

	persist_write_data(key, &workout, sizeof(workout));
}

static Workout getWorkout(int key) {
	Workout read_data;

	if (persist_exists(key))
		persist_read_data(key, &read_data, sizeof(read_data));

	return read_data;
}

static void workoutSelected(int index, void *ctx) {
	workout_menu[index].subtitle = "<< SELECTED >>";
	layer_mark_dirty(simple_menu_layer_get_layer(workout_menu_layer));
}

static void loadWorkoutMenu(void) {
	for(int w = 0; w < total_workouts; w++) {
		char *title = (char *)malloc(sizeof(char) * 10);
		snprintf(title, 10, "Workout %c", workouts[w].letter);
		workout_menu[w] = (SimpleMenuItem) {
			.title    = title,
			.subtitle = "Here be subtitles",
			.callback = workoutSelected,
			.icon     = gbitmap_create_with_resource(icon_ids[w])
		};
	}
	workout_sections[0] = (SimpleMenuSection) {
		.title     = "Today's workout is...",
		.num_items = total_workouts,
		.items     = workout_menu
	};
}

static void loadExercisesMenu(void) {
	
}

static void loadRepsScreen(void) {
	
}

static void loadIntervalScreen(void) {
	
}

static void loadExerciseDetailsScreen(void) {
	
}

static void begin(void) {
	setWorkout(1);
	setWorkout(2);
	workouts[0] = getWorkout(1);
	workouts[1] = getWorkout(2);

	window              = window_create();
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds        = layer_get_frame(window_layer);
	
	loadWorkoutMenu();
	workout_menu_layer = simple_menu_layer_create(bounds, window, workout_sections, 1, NULL);
	layer_add_child(window_layer, simple_menu_layer_get_layer(workout_menu_layer));

//	exercise_layer  = text_layer_create((GRect){ .origin = { 0, 0 }, .size = { .w = bounds.size.w, .h = bounds.size.h/2 } });
//	text_layer_set_font(exercise_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
//	text_layer_set_text_alignment(exercise_layer, GTextAlignmentCenter);

//	titles_layer  = text_layer_create((GRect){ .origin = { 0,                 (bounds.size.h/2) }, .size = { bounds.size.w/2, bounds.size.h/2 } });
//	numbers_layer = text_layer_create((GRect){ .origin = { (bounds.size.w/2)+5, (bounds.size.h/2) }, .size = { (bounds.size.w/2)-5, bounds.size.h/2 } });
//	text_layer_set_font(titles_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
//	text_layer_set_text_alignment(titles_layer, GTextAlignmentRight);
//	text_layer_set_font(numbers_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
//	text_layer_set_text_alignment(numbers_layer, GTextAlignmentLeft);

//	Workout my_workout = getWorkout(1);
//	Exercise my_ex = my_workout.exercises[2];
//
//	char numbers[15], title[100];
//	snprintf(numbers, sizeof(numbers), "%dx\n%d\n%dkg", my_ex.sets, my_ex.reps, my_ex.weight);
//	snprintf(title, sizeof(title), "[%c] %s", my_workout.letter, my_ex.name);
//	text_layer_set_text(exercise_layer, title);
//	text_layer_set_text(titles_layer, "Sets:\nReps:\nWeight:");
//	text_layer_set_text(numbers_layer, numbers);
//
//	layer_add_child(window_layer, text_layer_get_layer(exercise_layer));
//	layer_add_child(window_layer, text_layer_get_layer(titles_layer));
//	layer_add_child(window_layer, text_layer_get_layer(numbers_layer));
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
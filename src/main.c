#include <pebble.h>

#define MAX_WORKOUTS 7

typedef struct {
	char *name;
	int sets, reps, weight;
} Exercise;

typedef struct {
	char letter;
	Exercise *exercises;
} Workout;

int total_workouts = 0;
Workout workouts[MAX_WORKOUTS];

SimpleMenuSection workout_sections[1];
SimpleMenuItem workout_menu[MAX_WORKOUTS];

Window *window;
TextLayer *exercise_layer, *titles_layer, *numbers_layer;

static char itoc(int number) { return number + 64; }
static int ctoi(char letter) { return letter - 64; } 

static Exercise newExercise(char *name, int sets, int reps, int weight) {
	Exercise ex = { .name = name, .sets = sets, .reps = reps, .weight = weight };
	return ex;
}

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

static void loadMenu(void) {
	struct GBitmap *icon;
	unsigned int w;
	int icon_ids[] = {
		RESOURCE_ID_ICON_WORKOUT_A,
		RESOURCE_ID_ICON_WORKOUT_B,
		RESOURCE_ID_ICON_WORKOUT_C,
		RESOURCE_ID_ICON_WORKOUT_D,
		RESOURCE_ID_ICON_WORKOUT_E,
		RESOURCE_ID_ICON_WORKOUT_F,
		RESOURCE_ID_ICON_WORKOUT_G
	};
	for(w = 0; w <= sizeof(workouts); w++) {
		char title[10];
		Workout workout = workouts[w]; 
		icon = gbitmap_create_with_resource(icon_ids[ctoi(workout.letter)]);
		snprintf(title, sizeof(title), "Workout %c", workout.letter);
		workout_menu[w] = (SimpleMenuItem) {
			.title    = title,
			.subtitle = "Here be subtitles",
			.icon     = icon
		};
	}
	workout_sections[0] = (SimpleMenuSection) {
		.title     = "Workouts",
		.num_items = w,
		.items     = workout_menu
	};
}

static void workoutSelected(int index, void *ctx) {
	
}

static void begin(void) {
	setWorkout(1);
	workouts[0] = getWorkout(1);

	window              = window_create();
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds = layer_get_frame(window_layer);

	loadMenu();
	SimpleMenuLayer *menu_layer = simple_menu_layer_create(bounds, window, workout_sections, 1, NULL);
	layer_add_child(window_layer, simple_menu_layer_get_layer(menu_layer));	

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
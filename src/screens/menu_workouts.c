#include "../defs.h"
#include "../objects/Workout.h"
#include "menu_workouts.h"
#include "menu_exercises.h"

SimpleMenuItem    workout_menu[MAX_WORKOUTS];
SimpleMenuLayer  *workout_menu_layer;

void unloadWorkoutMenu(void) {
	simple_menu_layer_destroy(workout_menu_layer);
}

void loadWorkoutMenu(void) {
    APP_LOG(APP_LOG_LEVEL_INFO, "Loading Workout Menu");
	Window *window      = window_create();
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds        = layer_get_frame(window_layer);

	for(int w = 0; w < workouts_length; w++) {
		workouts[w] = getWorkout(w+1);
        APP_LOG(APP_LOG_LEVEL_DEBUG, "Created workout menu entry #%c", workouts[w].letter);
		char *title = (char *)malloc(sizeof(char) * 10);
		snprintf(title, 10, "Workout %c", workouts[w].letter);
		workout_menu[w] = (SimpleMenuItem) {
			.title    = title,
			.subtitle = workouts[w].description,
			.callback = loadExercisesMenu,
			.icon     = gbitmap_create_with_resource(icon_ids[w])
		};
	}

    APP_LOG(APP_LOG_LEVEL_INFO, "Creating Workout Menu with %d items", workouts_length);
	SimpleMenuSection sections[1] = {
		(SimpleMenuSection) {
			.title     = "Today's workout is...",
			.num_items = workouts_length,
			.items     = workout_menu
		}
	};

	workout_menu_layer = simple_menu_layer_create(bounds, window, sections, 1, NULL);
	layer_add_child(window_layer, simple_menu_layer_get_layer(workout_menu_layer));
//	window_set_window_handlers(window, (WindowHandlers) { .unload = unloadWorkoutMenu });
	window_stack_push(window, true);
}
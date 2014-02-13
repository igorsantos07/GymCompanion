#include <pebble.h>
#include "_defs.h"

SimpleMenuItem    workout_menu[MAX_WORKOUTS];
SimpleMenuLayer  *workout_menu_layer;
SimpleMenuItem    exercises_menu[MAX_EXERCISES];
SimpleMenuLayer  *exercises_menu_layer;

DataLoggingSessionRef logger;

static char itoc(uint16_t number) { return number + 64; }
static uint16_t ctoi(char letter) { return letter - 64; } 

static Exercise newExercise(uint16_t id, uint16_t workout_id, char *name, uint16_t sets, uint16_t reps, uint16_t weight, uint16_t interval) {
	Exercise ex = {
		.workout_id = workout_id,
		.name       = name,
		.sets       = sets,
		.reps       = reps,
		.weight     = weight,
		.interval   = interval
	};
	return ex;
}

static Workout newWorkout(uint16_t id, char *description, uint16_t exercises_length) {
	// TODO: should stop creating workouts when total_workouts = MAX_WORKOUTS and log that instead
	// TODO: this function should be syncronized with the workouts array!
	Workout w = {
		.id				  = id,
		.letter           = itoc(id),
		.description      = description,
		.exercises_length = exercises_length
	};
	++workouts_length;
	return w;
}

static Workout getWorkout(uint32_t id) {
	Workout read_data;

	if (persist_exists(id))
		persist_read_data(id, &read_data, sizeof(read_data));

	return read_data;
}

static int setWorkout(Workout workout) {
	int written = persist_write_data(workout.id, &workout, sizeof(workout));
	return written;
}

static uint32_t genExerciseKey(uint16_t workout_id, uint16_t exercise_id) {
	//TODO: should throw an error, since those numbers are bigger than we want
	if (exercise_id >= 100 || workout_id >= 10) return false;
	return (workout_id * 100) + exercise_id;
}

static Exercise getExercise(uint16_t workout_id, uint16_t id) {
	Exercise read_data;
	uint16_t key = genExerciseKey(workout_id, id);
		
	if (persist_exists(key))
		persist_read_data(key, &read_data, sizeof(read_data));
	
	return read_data;
}

static int setExercise(Exercise exercise) {
	uint16_t id = genExerciseKey(exercise.workout_id, exercise.id);
	int written = persist_write_data(id, &exercise, sizeof(exercise));
	return written;
}

static void loadRepsScreen(int index, void *ctx) {
	
}

static void unloadExerciseMenu(void) {
	simple_menu_layer_destroy(exercises_menu_layer);
}

static void loadExercisesMenu(int index, void *ctx) {
	Window *window      = window_create();
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds        = layer_get_frame(window_layer);
	static char *debug = "";

	Exercise ex1 = getExercise(workouts[index].id, 1);
	Exercise ex2 = getExercise(workouts[index].id, 2);
	snprintf(debug, 200, "Workout %c, total %d \nDesc: %s\n\nEx1 : %ux of %u reps, %ukg\n\nEx2: %ux of %u reps, %ukg",
		workouts[index].letter, workouts[index].exercises_length, workouts[index].description,
		ex1.sets, ex1.reps, ex1.weight,
		ex2.sets, ex2.reps, ex2.weight
	);
	TextLayer *exercise_layer = text_layer_create((GRect){ .origin = { 0, 0 }, .size = { .w = bounds.size.w, .h = bounds.size.h } });
	text_layer_set_text(exercise_layer, debug);
	layer_add_child(window_layer, text_layer_get_layer(exercise_layer));
	window_stack_push(window, true);
	
	
	
	
	//transformar "Exercise ex" num array de exercicios externo, assim como funciona com os workouts.
	//depois, se funcionar, a gente pergunta no fórum como otimizar isso
	//vai ser necessário zerar o array pra cada vez que essa tela for carregada, pra nao acumular os exercicios do workout anterior no atual
//	int e;
//	for(e = 1; e <= workouts[index].exercises_length; e++) {
//		Exercise ex = getExercise(workouts[index].id, e);
//		char *desc = (char *)malloc(50);
//		snprintf(desc, 50, "%ux %u, %ukg: %us rest", ex.sets, ex.reps, ex.weight, ex.interval);
//		exercises_menu[e-1] = (SimpleMenuItem) {
//			.title    = ex.name,
//			.subtitle = desc,
//			.callback = loadRepsScreen
//		};
//	}
//	
//	SimpleMenuSection ex_sections[1] = {
//		(SimpleMenuSection) {
//			.title     = "Choose first exercise:",
//			.num_items = e-1,
//			.items     = exercises_menu
//		}
//	};
//
//	exercises_menu_layer = simple_menu_layer_create(bounds, window, ex_sections, 1, NULL);
//	layer_add_child(window_layer, simple_menu_layer_get_layer(exercises_menu_layer));
////	window_set_window_handlers(window, (WindowHandlers) { .unload = unloadExerciseMenu });
//	window_stack_push(window, true);
}

static void unloadWorkoutMenu(void) {
	simple_menu_layer_destroy(workout_menu_layer);
}

static void loadWorkoutMenu(void) {
	Window *window      = window_create();
	Layer *window_layer = window_get_root_layer(window);
	GRect bounds        = layer_get_frame(window_layer); 

	for(int w = 0; w < workouts_length; w++) {
		workouts[w] = getWorkout(w+1);
		char *title = (char *)malloc(sizeof(char) * 10);
		snprintf(title, 10, "Workout %c", workouts[w].letter);
		workout_menu[w] = (SimpleMenuItem) {
			.title    = title,
			.subtitle = workouts[w].description,
			.callback = loadExercisesMenu,
			.icon     = gbitmap_create_with_resource(icon_ids[w])
		};
	}
	
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

//static void loadIntervalScreen(void) {
//	
//}

//static void loadExerciseDetailsScreen(void) {
//	
//}

static void setFakeWorkout(uint16_t id) {
	Workout workout;
	switch (id) {
		case 1:
			setExercise(newExercise(1, id, "Abdominal", 4, 20, 0, 45));
			setExercise(newExercise(2, id, "Supino", 4, 12, 15, 45));
			setExercise(newExercise(3, id, "Tríceps testa", 4, 12, 35, 45));
			workout = newWorkout(id, "Abdomen, chest, triceps", 3);
		break;
		
		case 2:
			setExercise(newExercise(1, id, "Bíceps concentrado", 4, 8, 12, 45));
			setExercise(newExercise(2, id, "Leg press 45o", 4, 10, 50, 60));
			setExercise(newExercise(3, id, "Agachamento Nakagym", 4, 10, 15, 60));
			setExercise(newExercise(4, id, "Mesa flexora", 4, 10, 45, 60));
			workout = newWorkout(id, "Biceps, legs", 4);
		break;
		
		default:
		case 3:
			setExercise(newExercise(1, id, "Puxada triângulo", 4, 10, 65, 45));
			setExercise(newExercise(2, id, "Remada 3 apoios", 4, 8, 18, 45));
			workout = newWorkout(id, "Shoulders, lower back", 2);
		break;
	}

	int written[] = { persist_write_data(id, &workout, sizeof(workout)) };
	data_logging_log(logger, written, 10);
}
static void fakeData(void) {
	setFakeWorkout(1);
	setFakeWorkout(2);
	setFakeWorkout(3);
}

static void begin(void) {
	logger = data_logging_create(42, DATA_LOGGING_INT, 4, false);
	fakeData();
	loadWorkoutMenu();
}

static void end(void) {
	data_logging_finish(logger);
}

int main(void) {
	begin(); app_event_loop(); end();
}
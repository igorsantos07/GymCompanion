#include "../defs.h"
#include "../objects/Workout.h"
#include "../objects/Exercise.h"
#include "menu_exercises.h"

SimpleMenuItem    exercises_menu[MAX_EXERCISES];
SimpleMenuLayer  *exercises_menu_layer;

void unloadExerciseMenu(void) {
	simple_menu_layer_destroy(exercises_menu_layer);
}

void loadExercisesMenu(int index, void *ctx) {
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

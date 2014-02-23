#include "defs.h"

#include "objects/Exercise.h"
#include "objects/Workout.h"

#include "screens/menu_workouts.h"
#include "screens/menu_exercises.h"

//static void loadRepsScreen(int index, void *ctx) {
//
//}

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

	int written = persist_write_data(id, &workout, sizeof(workout));
    APP_LOG(APP_LOG_LEVEL_INFO, "Written %dB to save workout #%d", written, id);
}
static void fakeData(void) {
	setFakeWorkout(1);
	setFakeWorkout(2);
	setFakeWorkout(3);
}

static void begin(void) {
	fakeData();
	loadWorkoutMenu();
    APP_LOG(APP_LOG_LEVEL_INFO, "Workout Menu loaded");
}

static void end(void) {

}

int main(void) {
	begin(); app_event_loop(); end();
}
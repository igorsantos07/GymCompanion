#include "Workout.h"
#include "../generic.h"

Workout newWorkout(uint16_t id, char *description, uint16_t exercises_length) {
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

Workout getWorkout(uint32_t id) {
	Workout read_data;

	if (persist_exists(id))
		persist_read_data(id, &read_data, sizeof(read_data));

	return read_data;
}

int setWorkout(Workout workout) {
	int written = persist_write_data(workout.id, &workout, sizeof(workout));
	return written;
}

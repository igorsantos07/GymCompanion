#include "Exercise.h"

Exercise newExercise(uint16_t id, uint16_t workout_id, char *name, uint16_t sets, uint16_t reps, uint16_t weight, uint16_t interval) {
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

uint32_t genExerciseKey(uint16_t workout_id, uint16_t exercise_id) {
	//TODO: should throw an error, since those numbers are bigger than we want
	if (exercise_id >= 100 || workout_id >= 10) return false;
	return (workout_id * 100) + exercise_id;
}

Exercise getExercise(uint16_t workout_id, uint16_t id) {
	Exercise read_data;
	uint16_t key = genExerciseKey(workout_id, id);

	if (persist_exists(key))
		persist_read_data(key, &read_data, sizeof(read_data));

	return read_data;
}

int setExercise(Exercise exercise) {
	uint16_t id = genExerciseKey(exercise.workout_id, exercise.id);
	int written = persist_write_data(id, &exercise, sizeof(exercise));
	return written;
}
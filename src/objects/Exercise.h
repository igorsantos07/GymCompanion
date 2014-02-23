#ifndef EXERCISE_H
    #define EXERCISE_H
    #include <pebble.h>

    #define MAX_EXERCISES 20

    typedef struct {
        char   *name;
        uint16_t id;
        uint16_t workout_id;
        uint16_t sets;
        uint16_t reps;
        uint16_t weight;
        uint16_t interval;
    } Exercise;
    //	} __attribute__((__packed__)) Exercise;

    Exercise newExercise(uint16_t id, uint16_t workout_id, char *name, uint16_t sets, uint16_t reps, uint16_t weight, uint16_t interval);

    uint32_t genExerciseKey(uint16_t workout_id, uint16_t exercise_id);

    Exercise getExercise(uint16_t workout_id, uint16_t id);

    int setExercise(Exercise exercise);

#endif
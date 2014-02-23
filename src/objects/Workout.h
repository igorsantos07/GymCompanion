#ifndef WORKOUT_H
    #define WORKOUT_H
    #include <pebble.h>

    #define MAX_WORKOUTS 7

    typedef struct {
        uint16_t id;
        char     letter;
        char    *description;
        uint16_t exercises_length;
    } Workout;

    extern int workouts_length;
    extern Workout workouts[MAX_WORKOUTS];

    Workout newWorkout(uint16_t id, char *description, uint16_t exercises_length);

    Workout getWorkout(uint32_t id);

    int setWorkout(Workout workout);

#endif
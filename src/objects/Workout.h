#ifndef WORKOUT_H
    #define WORKOUT_H
    #include <pebble.h>

    int workouts_length = 0;

    typedef struct {
        uint16_t id;
        char     letter;
        char    *description;
        uint16_t exercises_length;
    } Workout;

    Workout newWorkout(uint16_t id, char *description, uint16_t exercises_length);

    Workout getWorkout(uint32_t id);

    int setWorkout(Workout workout);

#endif
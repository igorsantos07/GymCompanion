#ifndef DEFINES
#define DEFINES

	#define MAX_WORKOUTS 7
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

	typedef struct {
		uint16_t id;
		char     letter;
		char    *description;
		uint16_t exercises_length;
	} Workout;

	int icon_ids[] = { RESOURCE_ID_ICON_WORKOUT_A, RESOURCE_ID_ICON_WORKOUT_B,
		RESOURCE_ID_ICON_WORKOUT_C, RESOURCE_ID_ICON_WORKOUT_D, RESOURCE_ID_ICON_WORKOUT_E,
		RESOURCE_ID_ICON_WORKOUT_F, RESOURCE_ID_ICON_WORKOUT_G
	};
	
	Workout workouts[MAX_WORKOUTS];
	int workouts_length = 0;
#endif
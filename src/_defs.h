#ifndef DEFINES
#define DEFINES

	#define MAX_WORKOUTS 7

	typedef struct {
		char *name;
		int   sets,
		      reps,
		      weight;
	} Exercise;

	typedef struct {
		char     letter;
		char    *description;
		Exercise *exercises;
	} Workout;

	int icon_ids[] = { RESOURCE_ID_ICON_WORKOUT_A, RESOURCE_ID_ICON_WORKOUT_B,
		RESOURCE_ID_ICON_WORKOUT_C, RESOURCE_ID_ICON_WORKOUT_D, RESOURCE_ID_ICON_WORKOUT_E,
		RESOURCE_ID_ICON_WORKOUT_F, RESOURCE_ID_ICON_WORKOUT_G
	};
	
	Workout workouts[MAX_WORKOUTS];
	int total_workouts = 0;
#endif
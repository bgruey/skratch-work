/*
    Movement based on the signal from the drum trigger.

    On for 15 ms once triggered.
*/
#include <math.h>
#include <time.h>
#include <unistd.h>
#include "now.h"

// User defined inputs
#define ON_THRESHOLD 0.8

typedef struct {
    double start_time;
    double local_now;
} MoveSignalState_t;


double get_move_signal(double y, TimeWFloat_t* now, MoveSignalState_t* state);
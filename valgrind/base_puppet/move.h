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

#ifndef MOVE_SIGNAL_STATE_T
#define MOVE_SIGNAL_STATE_T

typedef struct {
    double start_time;
    double local_now;
} MoveSignalState_t;


double get_move_signal(double y, TimeWFloat_t* now, MoveSignalState_t* state);

#endif

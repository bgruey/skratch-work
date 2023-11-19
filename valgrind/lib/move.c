/*
    Movement based on the signal from the drum trigger.
*/
#include "move.h"

double get_move_signal(double y, TimeWFloat_t* now, MoveSignalState_t* state) {
    // run on for 15 ms, then off
    if (state->start_time > 0.0) {
        state->local_now = now->seconds - state->start_time;
        if (state->local_now < 0.015)
            return 1.0;
        state->start_time = 0.0;
        return 0.0;
    }

    // start movement function
    if(ON_THRESHOLD < y) {
        state->start_time = now->seconds;
        return 1;
    }

    return 0.0;
}

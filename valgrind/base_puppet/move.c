/*
    Movement based on the signal from the drum trigger.
*/
#include "move.h"

double get_move_signal(double y, TimeWFloat_t* now, MoveSignalState_t* state) {
    
    state->local_now = now->seconds - state->start_time;
    //printf("%f: %f > %f\n", state->start_time, y, ON_THRESHOLD);
    // run on for 15 ms, then off
    // if (state->start_time > 0.0) {
    //     if (state->local_now < 0.015)
    //         return 1.0;
    // }

    // start movement function
    if(ON_THRESHOLD < y) {
        state->start_time = now->seconds;
        return 1.0;
    }

    state->start_time = 0.0;
    return 0.0;
}

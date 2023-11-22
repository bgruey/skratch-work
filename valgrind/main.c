#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "dancer/dancer.h"
#include "pin_threads/pin_thread.h"

#include "base_puppet/now.h"
#include "base_puppet/move.h"
#include "base_puppet/drum_signal.h"
#include "base_puppet/sleep.h"

int main(int argc, char** argv) {
    //int num_points = 75001;
    struct timespec* sleep_data = (struct  timespec*)calloc(1, sizeof(struct timespec));

    /*
        In testing, channel definitions:
            0: time
            1: kick
            2: snare

        To do:
            - Locked state at the end, need to fix that
            - Build integrator with resistor shunt
            - Build python plotter instead of libreoffice calc
    */
    DancerState_t* dancer = initialize_dancer(
        2, 2, 5,
        "./data.csv"
    );

    int i;
    while (dancer->pin_reader_thread_data->run_bool) {
        step_forward_buffer(dancer);

        // sleep_via_double(0.0001, sleep_data);
        if ((i % 10000) == 0)
            printf("%d: now\n", i);
        i+= 1;
    }


    destroy_dancer(dancer);
    free(sleep_data);
    return 0;
}

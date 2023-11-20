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
    int num_points = 300;
    struct timespec* sleep_data = (struct  timespec*)calloc(1, sizeof(struct timespec));

    DancerState_t* dancer = initialize_dancer(
        4, 4, 5,
        "./data.csv"
    );

    int i;
    for (i = 0; i < num_points; i++) {
        step_forward_buffer(dancer);
        sleep_via_double(0.002, sleep_data);
    }

    dancer->pin_reader_thread_data->run_bool[0] = 0;
    dancer->pin_writer_thread_data->run_bool[0] = 0;
    pthread_join(dancer->pin_reader_thread_data->thread[0], NULL);
    pthread_join(dancer->pin_writer_thread_data->thread[0], NULL);
    
    free_dancer(dancer);
    free(sleep_data);
    return 0;
}

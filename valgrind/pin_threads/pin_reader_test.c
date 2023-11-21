/*
    Placeholder, read data from pins (create for now)
*/
#include <stdio.h>
#include "pin_thread.h"

size_t read_timestamp(FILE* f, double* t, int* y) {
    size_t nread = fread(t, sizeof(double), 1, f);
    if (nread != 1)
        return 0;
    return fread(y, sizeof(int), 1, f) + nread;
}

void* pin_reader_test(void* args_in) {
    PinThreadData_t* args = (PinThreadData_t*)args_in; 

    TimeWFloat_t* now = (TimeWFloat_t*)calloc(1, sizeof(TimeWFloat_t));
    now->start_time_seconds = get_now_seconds(now);
    struct timespec* sleep_data = (struct timespec*)calloc(1, sizeof(struct timespec));

    FILE* waveforms = fopen("waveforms.dat", "rb");
    double ts;
    int y;
    size_t num_read = 0;

    unsigned short pin_index;
    num_read = read_timestamp(waveforms, &ts, &y);
    while (args->run_bool[0]) {
            // printf(
            //     "%f < %f --> %d\n",
            //     ts,
            //     get_now_seconds(now),
            //     y
            // );
        // Mocking the adc, must read data until we catch up
        while (ts < get_now_seconds(now)) {
            num_read = read_timestamp(waveforms, &ts, &y);
        }

        if (num_read == 0) {
            args->run_bool[0] = 0;
            continue;
        }

        // Mocking analog, sleep until we catchup to the quantized time series
        if (ts > get_now_seconds(now))
            sleep_via_double(
                ts - now->seconds - now->start_time_seconds,
                sleep_data
            );

        for (pin_index = 0; pin_index < args->num_pins; pin_index++)
            args->pins[pin_index] = (double)y;
        
        // Pretend to be an adc, no update for this long
        sleep_via_double(
            0.001,  // 860 samples / s, adc limit
            sleep_data
        );
    }

    free(now);
    free(sleep_data);
    fclose(waveforms);
    return NULL;
}

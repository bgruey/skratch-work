/*
    Movement based on the signal from the drum trigger.
*/
#include <stdio.h>
#include "pin_reader.h"
#include "../base_puppet/sleep.h"

void* pin_reader(void* args_in) {\
    PinReaderThreadData_t* args = (PinReaderThreadData_t*)args_in; 

    TimeWFloat_t* now = (TimeWFloat_t*)calloc(1, sizeof(TimeWFloat_t));
    struct timespec* sleep_data = (struct  timespec*)calloc(1, sizeof(struct timespec));

    DrumSignalData_t* drum_signal_data = setup_signal_data(
        0.001, 0.01, 0.005, 0.14
    );

    double t, y;
    unsigned short pin_index;
    while (args->run_bool[0]) {
        t = get_now_seconds_fraction(now, 0.5);

        y = get_drum_signal(t, drum_signal_data);

        for (pin_index = 0; pin_index < args->num_pins; pin_index++)
            args->pins[pin_index] = (1.0 + 0.25 * pin_index) * y;
        sleep_via_double(0.001, sleep_data);
    }

    free(now);
    free(sleep_data);
    free(drum_signal_data);
    return NULL;
}

int launch_pin_reader_thread(
    PinReaderThreadData_t* reader_data
) {
    int err = pthread_create(reader_data->thread, NULL, pin_reader, reader_data);
    if (err != 0)
        return err;

    return 0;
}
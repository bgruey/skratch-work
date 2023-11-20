/*
    Movement based on the signal from the drum trigger.
*/
#include <stdio.h>
#include "pin_reader.h"
#include "../base_puppet/sleep.h"

void* pin_reader(void* args_in) {

    PinReaderArg* args = (PinReaderArg*)calloc(1, sizeof(PinReaderArg));
    args->num_pins = ((PinReaderArg*)args_in)->num_pins;
    args->run_bool = ((PinReaderArg*)args_in)->run_bool;
    args->pins = ((PinReaderArg*)args_in)->pins;

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
    free(drum_signal_data);
    return NULL;
}

int launch_pin_reader_thread(
    double* pin_states,
    unsigned short num_pins,
    int* run_bool,
    pthread_t* thread
) {
    struct timespec* sleep_data = (struct  timespec*)calloc(1, sizeof(struct timespec));

    PinReaderArg* args = (PinReaderArg*)calloc(1, sizeof(PinReaderArg));
    args->num_pins = num_pins;
    args->pins = pin_states;
    args->run_bool = run_bool;

    int err = pthread_create(thread, NULL, pin_reader, args);
    if (err != 0)
        return err;

    printf("Sleeping while thread copies date over.\n");
    sleep_via_double(0.5, sleep_data);
    printf("FInished sleeping.\n");

    return 0;
}
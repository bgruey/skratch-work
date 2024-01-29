/*
    Placeholder, read data from pins (create for now)
*/
#include "pin_thread.h"

void* pin_reader(void* args_in) {
    PinThreadData_t* args = (PinThreadData_t*)args_in; 

    TimeWFloat_t* now = (TimeWFloat_t*)calloc(1, sizeof(TimeWFloat_t));
    struct timespec* sleep_data = (struct  timespec*)calloc(1, sizeof(struct timespec));

    DrumSignalData_t* drum_signal_data = setup_signal_data(
        0.001, 0.01, 0.005, 0.14
    );

    unsigned short pin_index;
    while (args->run_bool) {

        for (pin_index = 0; pin_index < args->num_pins; pin_index++)
            args->pins[pin_index] = read_pin(
                pin_index,
                get_drum_signal(
                    get_now_seconds_fraction(now, 0.5),
                    drum_signal_data
                ) 
            );
        sleep_via_double(0.0001, sleep_data);
    }

    free(now);
    free(sleep_data);
    free(drum_signal_data);
    return NULL;
}

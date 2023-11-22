/*
    Placeholder
*/
#include "pin_thread.h"

void* pin_writer(void* args_in) {
    PinThreadData_t* args = (PinThreadData_t*)args_in; 

    struct timespec* sleep_data = (struct  timespec*)calloc(1, sizeof(struct timespec));

    unsigned short pin_index;
    while (args->run_bool) {

        for (pin_index = 0; pin_index < args->num_pins; pin_index++)
            write_pin(pin_index, args->pins[pin_index]);

        sleep_via_double(0.0001, sleep_data);
    }

    free(sleep_data);
    return NULL;
}

/*
    Read pin and update dancer state
*/

#include <pthread.h>
#include "../base_puppet/now.h"
#include "../base_puppet/drum_signal.h"


typedef struct {
    unsigned short num_pins;
    double* pins;
    int* run_bool;
} PinReaderArg;


void* pin_reader(void* args_in);

int launch_pin_reader_thread(
    double* pin_states,
    unsigned short num_pins,
    int* run_bool,
    pthread_t* thread
);

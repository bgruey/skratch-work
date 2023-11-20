/*
    Read pin and update dancer state
*/

#include <pthread.h>
#include "../base_puppet/now.h"
#include "../base_puppet/drum_signal.h"

#ifndef PIN_READER_THREAD_H
#define PIN_READER_THREAD_H

typedef struct {
    unsigned short num_pins;
    double* pins;
    int* run_bool;
    pthread_t* thread;
} PinReaderThreadData_t;


void* pin_reader(void* args_in);

int launch_pin_reader_thread(
    PinReaderThreadData_t* pin_reader_thread_data
);

#endif

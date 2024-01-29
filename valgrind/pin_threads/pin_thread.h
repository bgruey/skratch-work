#ifndef PIN_THREAD_H
#define PIN_THREAD_H
/*
    Read pin and update dancer state
*/

#include <pthread.h>
#include "../base_puppet/now.h"
#include "../base_puppet/sleep.h"
#include "../base_puppet/drum_signal.h"

typedef struct {
    unsigned short num_pins;
    double* pins;
    int run_bool;
    pthread_t thread;
    pthread_mutex_t* read_now_mutex;
    pthread_cond_t* read_now_cond;
    int* read_now;
    double dt; // timestep for reader in testing.
} PinThreadData_t;


void* pin_reader(void* args_in);
void* pin_reader_test(void* args_in);
void* pin_writer(void* args_in);

int launch_pin_thread(
    pthread_attr_t* attr,
    PinThreadData_t* pin_reader_thread_data,
    void* (*thread_target)(void *)
);

double read_pin(unsigned short pin_id, double y);
void write_pin(unsigned short pin_id, double y);

#endif

#ifndef DANCER_H
#define DANCER_H
/*
    Program state.
*/
#include <stdio.h>
#include <pthread.h>
#include "../base_puppet/now.h"
#include "../base_puppet/move.h"
#include "../pin_threads/pin_thread.h"

typedef struct {
    TimeWFloat_t* now;
    double start_time_seconds;
    MoveSignalState_t* move_state;

    unsigned short num_read_pins;
    unsigned short read_pin_i;
    double* read_pins;

    unsigned short num_write_pins;
    unsigned short write_pin_i;
    double* write_pins;

    unsigned short len_buffer;
    double** state_buffer;
    unsigned short buffer_i;
    unsigned short next_buffer_i;

    FILE* outfile;
    char* line_buffer;
    unsigned short len_line;

    int read_now;
    int write_now;
    pthread_attr_t pthread_attr;
    pthread_mutex_t read_now_mutex;
    pthread_cond_t read_now_cond;

    PinThreadData_t* pin_reader_thread_data;
    PinThreadData_t* pin_writer_thread_data;
} DancerState_t;


DancerState_t* initialize_dancer(
    unsigned short num_read_pins,
    unsigned short num_write_pins,
    unsigned short len_buffer,
    const char* out_filename
);

void destroy_dancer(DancerState_t* dancer);

void step_forward_buffer(DancerState_t* dancer);


// Low level access functions
double* get_current_state(DancerState_t* dancer);
double* get_next_state(DancerState_t* dancer);

// Current state, used outside of main thread update routine
double get_current_read_pin_state(DancerState_t* dancer);
double get_current_read_pin_state_i(DancerState_t* dancer, unsigned short i);
void set_current_read_pin_state(DancerState_t* dancer, double value);

double get_current_write_pin_state(DancerState_t* dancer);
void set_current_write_pin_state(DancerState_t* dancer, double value);

// Next state (used for updating by main thread)
double get_next_read_pin_state(DancerState_t* dancer);
double get_next_read_pin_state_i(DancerState_t* dancer, unsigned short i);
void set_next_read_pin_state(DancerState_t* dancer, double value);

double get_next_write_pin_state(DancerState_t* dancer);
void set_next_write_pin_state(DancerState_t* dancer, double value);

#endif

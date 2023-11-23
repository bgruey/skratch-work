#include "dancer.h"


double* get_current_state(DancerState_t* dancer) {
    return dancer->state_buffer[dancer->buffer_i];
}
double* get_next_state(DancerState_t* dancer) {
    return dancer->state_buffer[dancer->next_buffer_i];
}

// Current state, used outside of main thread update routine
double get_current_read_pin_state(DancerState_t* dancer) {
    return get_current_state(dancer)[dancer->read_pin_i]; 
}

double get_current_read_pin_state_i(DancerState_t* dancer, unsigned short i) {
    return get_current_state(dancer)[i]; 
}

void set_current_read_pin_state(DancerState_t* dancer, double value) {
    get_current_state(dancer)[dancer->read_pin_i] = value;
}

double get_current_write_pin_state(DancerState_t* dancer) {
    return get_current_state(dancer)[dancer->num_read_pins + dancer->write_pin_i]; 
}

void set_current_write_pin_state(DancerState_t* dancer, double value) {
    get_current_state(dancer)[dancer->num_read_pins + dancer->write_pin_i] = value;
}


// Next state (used for updating by main thread)
double get_next_read_pin_state(DancerState_t* dancer) {
    return get_next_state(dancer)[dancer->read_pin_i]; 
}

double get_next_read_pin_state_i(DancerState_t* dancer, unsigned short i) {
    return get_next_state(dancer)[i]; 
}

void set_next_read_pin_state(DancerState_t* dancer, double value) {
    get_next_state(dancer)[dancer->read_pin_i] = value;
}

double get_next_write_pin_state(DancerState_t* dancer) {
    return get_next_state(dancer)[dancer->num_read_pins + dancer->write_pin_i]; 
}

void set_next_write_pin_state(DancerState_t* dancer, double value) {
    get_next_state(dancer)[dancer->num_read_pins + dancer->write_pin_i] = value;
}

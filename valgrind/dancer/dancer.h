#include <stdio.h>
#include "../base_puppet/now.h"
#include "../base_puppet/move.h"
#include "../pin_threads/pin_reader.h"

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

    PinReaderThreadData_t* pin_reader_thread_data;
} DancerState_t;

DancerState_t* initialize_dancer(
    unsigned short num_read_pins,
    unsigned short num_write_pins,
    unsigned short len_buffer,
    const char* out_filename
);

void free_dancer(DancerState_t* dancer);

void step_forward_buffer(DancerState_t* dancer);
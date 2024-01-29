/*

    Sources:
        - explicitly joinable, mutex: https://hpc-tutorials.llnl.gov/posix/example_using_cond_vars/

*/
#include "dancer.h"


PinThreadData_t* init_pin_thread(
    DancerState_t* dancer,
    unsigned short num_pins,
    double* pins,
    void* (*thread_target)(void *)
) {
    PinThreadData_t* ret = (PinThreadData_t*)calloc(1, sizeof(PinThreadData_t));
    ret->num_pins = num_pins;
    ret->pins = pins;
    ret->run_bool = 1;
    ret->read_now = &dancer->read_now;
    ret->read_now_mutex = &dancer->read_now_mutex;
    ret->read_now_cond = &dancer->read_now_cond;

    int err = launch_pin_thread(
        &dancer->pthread_attr,
        ret,
        thread_target
    );

    if (err) {
        fprintf(stderr, "Error creating reader thread: %d", err);
        exit(1);
    }
    return ret;
}


DancerState_t* initialize_dancer(
    unsigned short num_read_pins,
    unsigned short num_write_pins,
    unsigned short len_buffer,
    const char* out_filename
) {
    if (num_read_pins != num_write_pins) {
        fprintf(
            stderr,
            "ERROR: Mismatched number of pins. Read and write must match.\n"
        );
        exit(EXIT_FAILURE);
    }
    if (len_buffer < 2) {
        fprintf(
            stderr,
            "ERROR: Buffer length less than 2.\n"
        );
        exit(EXIT_FAILURE);
    }

    DancerState_t* dancer = (DancerState_t*)calloc(1, sizeof(DancerState_t));

    dancer->now = (TimeWFloat_t*)calloc(1, sizeof(TimeWFloat_t));
    dancer->move_state = (MoveSignalState_t*)calloc(1, sizeof(MoveSignalState_t));
    dancer->num_read_pins = num_read_pins;
    dancer->num_write_pins = num_write_pins;
    dancer->len_buffer = len_buffer;

    fprintf(
        stderr, 
        "Additional pins added in dancer/init.c, ~line 69.\n"
    );
    dancer->num_read_pins += 1;  // added for timestamp
    dancer->read_pins = (double*)calloc(dancer->num_read_pins, sizeof(double));
    dancer->write_pins = (double*)calloc(2*dancer->num_write_pins, sizeof(double));  // added for schmidt trigger signals
    dancer->state_buffer = (double**)calloc(dancer->len_buffer, sizeof(double*));

    unsigned short i;
    for (i = 0; i < len_buffer; i++)
        dancer->state_buffer[i] = (double*)calloc(
            1 + num_read_pins + 2*num_write_pins,  // extra states for schmidt triggers
            sizeof(double)
        );

    dancer->outfile = fopen(out_filename, "w");
    dancer->line_buffer = (char*)calloc(4096, sizeof(char));

    dancer->start_time_seconds = get_now_seconds(dancer->now);

    dancer->read_now = 0;
    pthread_mutex_init(&dancer->read_now_mutex, NULL);
    pthread_cond_init(&dancer->read_now_cond, NULL);

    pthread_attr_init(&dancer->pthread_attr);
    pthread_attr_setdetachstate(&dancer->pthread_attr, PTHREAD_CREATE_JOINABLE);

    dancer->pin_reader_thread_data = init_pin_thread(
        dancer,
        dancer->num_read_pins,
        dancer->read_pins,
        pin_reader_test
    );

    dancer->pin_writer_thread_data = init_pin_thread(
        dancer,
        dancer->num_write_pins,
        dancer->write_pins,
        pin_writer
    );

    dancer->schmidt_triggers = (SchmidtTrigger_T**)calloc(
        dancer->num_write_pins, 
        sizeof(SchmidtTrigger_T*)
    );
    for(i = 0; i < dancer->num_write_pins; i++)
        dancer->schmidt_triggers[i] = schmtt_init(
            0.4, 0.1, 0.4, 0.05
        );

    return dancer;
}

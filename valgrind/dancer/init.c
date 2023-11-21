#include "dancer.h"

DancerState_t* initialize_dancer(
    unsigned short num_read_pins,
    unsigned short num_write_pins,
    unsigned short len_buffer,
    const char* out_filename
) {
    int err;
    if (num_read_pins != num_write_pins) {
        fprintf(
            stderr,
            "ERROR: Mismatched number of pins. Read and write must match."
        );
        exit(EXIT_FAILURE);
    }

    DancerState_t* dancer = (DancerState_t*)calloc(1, sizeof(DancerState_t));

    dancer->now = (TimeWFloat_t*)calloc(1, sizeof(TimeWFloat_t));
    dancer->move_state = (MoveSignalState_t*)calloc(1, sizeof(MoveSignalState_t));
    dancer->num_read_pins = num_read_pins;
    dancer->num_write_pins = num_write_pins;
    dancer->len_buffer = len_buffer;

    dancer->read_pins = (double*)calloc(num_read_pins, sizeof(double));
    dancer->write_pins = (double*)calloc(num_write_pins, sizeof(double));
    dancer->state_buffer = (double**)calloc(len_buffer, sizeof(double*));

    unsigned short i;
    for (i = 0; i < len_buffer; i++)
        dancer->state_buffer[i] = (double*)calloc(
            1 + num_read_pins + num_write_pins,
            sizeof(double)
        );

    dancer->outfile = fopen(out_filename, "w");
    dancer->line_buffer = (char*)calloc(4096, sizeof(char));


    dancer->start_time_seconds = get_now_seconds(dancer->now);

    dancer->pin_reader_thread_data = (PinThreadData_t*)calloc(1, sizeof(PinThreadData_t));
    dancer->pin_reader_thread_data->num_pins = dancer->num_read_pins;
    dancer->pin_reader_thread_data->pins = dancer->read_pins;
    dancer->pin_reader_thread_data->run_bool = (int*)calloc(1, sizeof(int));
    dancer->pin_reader_thread_data->run_bool[0] = 1;
    dancer->pin_reader_thread_data->thread = (pthread_t*)calloc(1, sizeof(pthread_t));

    err = launch_pin_thread(
        dancer->pin_reader_thread_data,
        pin_reader_test
    );
    if (err) {
        fprintf(stderr, "Error creating reader thread: %d", err);
        exit(1);
    }

    dancer->pin_writer_thread_data = (PinThreadData_t*)calloc(1, sizeof(PinThreadData_t));
    dancer->pin_writer_thread_data->num_pins = dancer->num_write_pins;
    dancer->pin_writer_thread_data->pins = dancer->write_pins;
    dancer->pin_writer_thread_data->run_bool = (int*)calloc(1, sizeof(int));
    dancer->pin_writer_thread_data->run_bool[0] = 1;
    dancer->pin_writer_thread_data->thread = (pthread_t*)calloc(1, sizeof(pthread_t));

    err = launch_pin_thread(
        dancer->pin_writer_thread_data,
        pin_writer
    );
    if (err) {
        fprintf(stderr, "Error creating reader thread: %d", err);
        exit(1);
    }

    return dancer;
}

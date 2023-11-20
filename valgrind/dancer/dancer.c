#include <unistd.h>
#include <stdlib.h>

#include "dancer.h"
#include "../base_puppet/move.h"
#include "../base_puppet/now.h"
#include "../pin_threads/pin_reader.h"

DancerState_t* initialize_dancer(
    unsigned short num_read_pins,
    unsigned short num_write_pins,
    unsigned short len_buffer,
    const char* out_filename
) {
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

    dancer->pin_reader_thread_data = (PinReaderThreadData_t*)calloc(1, sizeof(PinReaderThreadData_t));
    dancer->pin_reader_thread_data->num_pins = dancer->num_read_pins;
    dancer->pin_reader_thread_data->pins = dancer->read_pins;
    dancer->pin_reader_thread_data->run_bool = (int*)calloc(1, sizeof(int));
    dancer->pin_reader_thread_data->run_bool[0] = 1;
    dancer->pin_reader_thread_data->thread = (pthread_t*)calloc(1, sizeof(pthread_t));

    return dancer;
}


void free_dancer(DancerState_t* dancer) {
    free(dancer->now);
    free(dancer->move_state);

    free(dancer->read_pins);
    free(dancer->write_pins);

    unsigned short i;
    for (i = 0; i < dancer->len_buffer; i++)
        free(dancer->state_buffer[i]);
    
    free(dancer->state_buffer);

    fclose(dancer->outfile);
    free(dancer->line_buffer);

    free(dancer->pin_reader_thread_data->run_bool);
    free(dancer->pin_reader_thread_data->thread);
    free(dancer->pin_reader_thread_data);

    free(dancer);
}


double* get_current_state(DancerState_t* dancer) {
    return dancer->state_buffer[dancer->buffer_i];
}

double get_current_read_pin_state(DancerState_t* dancer) {
    return get_current_state(dancer)[1 + dancer->read_pin_i]; 
}

double get_current_read_pin_state_i(DancerState_t* dancer, unsigned short i) {
    return get_current_state(dancer)[1 + i]; 
}

void set_current_read_pin_state(DancerState_t* dancer, double value) {
    get_current_state(dancer)[1 + dancer->read_pin_i] = value;
}

double get_current_write_pin_state(DancerState_t* dancer) {
    return get_current_state(dancer)[1 + dancer->num_read_pins + dancer->write_pin_i]; 
}

void set_current_write_pin_state(DancerState_t* dancer, double value) {
    get_current_state(dancer)[1 + dancer->num_read_pins + dancer->write_pin_i] = value;
}


void step_forward_buffer(DancerState_t* dancer) {
    get_now_seconds_fraction(dancer->now, 0.5);

    dancer->buffer_i = (dancer->buffer_i + 1) % dancer->len_buffer;

    dancer->state_buffer[dancer->buffer_i][0] = dancer->now->seconds;

    dancer->line_buffer[0] = '\0';
    dancer->len_line = sprintf(
        dancer->line_buffer,
        "%.5f",
        dancer->state_buffer[dancer->buffer_i][0]
    );

    for (dancer->read_pin_i = 0; dancer->read_pin_i < dancer->num_read_pins; dancer->read_pin_i++) {
        set_current_read_pin_state(
            dancer,
            dancer->read_pins[dancer->read_pin_i]
        );

        dancer->len_line += sprintf(
            dancer->line_buffer + dancer->len_line,
            ",%.3e",
            get_current_read_pin_state(dancer)
        );
    }    

    for (dancer->write_pin_i = 0; dancer->write_pin_i < dancer->num_write_pins; dancer->write_pin_i++) {
        set_current_write_pin_state(
            dancer,
            get_move_signal(
                get_current_read_pin_state_i(dancer, dancer->write_pin_i),
                dancer->now,
                dancer->move_state
            )
        );

        dancer->len_line += sprintf(
            dancer->line_buffer + dancer->len_line,
            ",%.3e",
            get_current_write_pin_state(dancer)
        );
    }

    fprintf(
        dancer->outfile,
        "%s\n",
        dancer->line_buffer
    );

}

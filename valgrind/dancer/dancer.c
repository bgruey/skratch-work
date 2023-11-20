#include <unistd.h>
#include <stdlib.h>

#include "dancer.h"
#include "../base_puppet/move.h"
#include "../base_puppet/now.h"

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

    free(dancer->outfile);
    free(dancer->line_buffer);
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
        dancer->state_buffer[dancer->buffer_i][1 + dancer->read_pin_i]
            = dancer->read_pins[dancer->read_pin_i];
        dancer->len_line += sprintf(
            dancer->line_buffer,
            ",%.3e",
            dancer->state_buffer[dancer->buffer_i][1 + dancer->read_pin_i]
        );
    }    

    for (dancer->write_pin_i = 0; dancer->write_pin_i < dancer->num_write_pins; dancer->write_pin_i++) {
        dancer->state_buffer[dancer->buffer_i][1 + dancer->num_read_pins + dancer->write_pin_i]
            = get_move_signal(
                dancer->state_buffer[dancer->buffer_i][1 + dancer->write_pin_i],
                dancer->now,
                dancer->move_state
            );
        dancer->len_line += sprintf(
            dancer->line_buffer,
            ",%.3e",
            dancer->state_buffer[dancer->buffer_i][1 + dancer->num_read_pins + dancer->read_pin_i]
        );
    }

    fprintf(
        dancer->outfile,
        "%s\n",
        dancer->line_buffer
    );

}
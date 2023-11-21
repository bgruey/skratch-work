#include <math.h>
#include "dancer.h"
#include "low_pass_filter.h"

#define LOW_PASS_F 50.0
#define M_PI 3.14159265358979323846

void step_forward_buffer(DancerState_t* dancer) {
    /*
        Current means the time step that was last completed.
        Next means the time step we are currently calculating.

        This ensures that the read/write threads can continue to access
        the current state and break synchronicity with this calculation
        thread.
    */

    get_now_seconds_fraction(dancer->now, 0.5);
    double alpha, dt;

    dancer->next_buffer_i = (dancer->buffer_i + 1) % dancer->len_buffer;

    dancer->state_buffer[dancer->buffer_i][0] = dancer->now->seconds;
    dt = get_next_state(dancer)[0] - get_current_state(dancer)[0];

    dancer->line_buffer[0] = '\0';
    dancer->len_line = sprintf(
        dancer->line_buffer,
        "%.5f",
        dancer->state_buffer[dancer->buffer_i][0]
    );

    for (dancer->read_pin_i = 0; dancer->read_pin_i < dancer->num_read_pins; dancer->read_pin_i++) {
        set_next_read_pin_state(
            dancer,
            dancer->read_pins[dancer->read_pin_i]
        );

        dancer->len_line += sprintf(
            dancer->line_buffer + dancer->len_line,
            ",%.3e",
            get_next_read_pin_state(dancer)
        );
    }    

    alpha = LOW_PASS_F * 2 * M_PI * 0.001;
    alpha = alpha / (1.0 + alpha);
    for (dancer->write_pin_i = 0; dancer->write_pin_i < dancer->num_write_pins; dancer->write_pin_i++) {
        set_next_write_pin_state(
            dancer,
            low_pass_filter(
                get_next_read_pin_state_i(dancer, dancer->write_pin_i),
                get_current_write_pin_state(dancer),
                alpha
            )
        );

        dancer->len_line += sprintf(
            dancer->line_buffer + dancer->len_line,
            ",%.3e",
            get_next_write_pin_state(dancer)
        );
    }

    fprintf(
        dancer->outfile,
        "%s\n",
        dancer->line_buffer
    );

    dancer->buffer_i = dancer->next_buffer_i;

}

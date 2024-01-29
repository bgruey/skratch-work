#include <math.h>
#include "dancer.h"
#include "signal_processors.h"

#define LOW_PASS_F 50.0
#define M_PI 3.14159265358979323846

void step_forward_buffer(DancerState_t* dancer) {
    /*
        Current means the time step that was last completed.
        Next means the time step we are currently calculating.

        This ensures that the read/write threads can continue to access
        the current state and break synchronicity with this calculation
        thread.

        Mutex in testing:
            - In order to ensure this happens serially, we lock for all calculations
            - in practice with streaming we will only want to lock for the read from the pins

        Perhaps double ptr the read pins passed to the reader thread so it actually maps
        to the current read pin state directly?
    */

    get_now_seconds_fraction(dancer->now, 0.5);
    double alpha;


    dancer->next_buffer_i = (dancer->buffer_i + 1) % dancer->len_buffer;
    dancer->read_now = 1;
    pthread_cond_signal(&dancer->read_now_cond);
    pthread_mutex_lock(&dancer->read_now_mutex);
    while(dancer->read_now == 1 && dancer->pin_reader_thread_data->run_bool)
        pthread_cond_wait(&dancer->read_now_cond, &dancer->read_now_mutex);

    dancer->line_buffer[0] = '\0';
    dancer->len_line = 0;

    alpha = LOW_PASS_F * 2 * M_PI * dancer->pin_reader_thread_data->dt;
    alpha = alpha / (1.0 + alpha);
    for (dancer->read_pin_i = 0; dancer->read_pin_i < dancer->num_read_pins; dancer->read_pin_i++) {
        set_next_read_pin_state(
            dancer,
            dancer->read_pins[dancer->read_pin_i]
        );
    }
    dancer->len_line += sprintf(
        dancer->line_buffer + dancer->len_line,
        "%.13e,%.5e,%.5e,",
        get_next_state(dancer)[2],  // time
        get_next_state(dancer)[0],  // kick signal
        get_next_state(dancer)[1]  // snare signal
    );

    for (dancer->write_pin_i = 0; dancer->write_pin_i < dancer->num_write_pins; dancer->write_pin_i++) {
        get_next_state(dancer)[dancer->num_read_pins + dancer->write_pin_i] = shunted_integrator(
                get_current_state(dancer)[dancer->num_read_pins + dancer->write_pin_i],  // Current integral
                get_next_state(dancer)[dancer->write_pin_i],  // add 1 to skip over time value
                dancer->pin_reader_thread_data->dt,
                100.0  // lambda, decays to 1/e 100 times per second.
        );

        get_next_state(dancer)[dancer->num_read_pins + dancer->num_write_pins + dancer->write_pin_i] = schmtt_calculate(
            get_next_state(dancer)[dancer->num_read_pins + dancer->write_pin_i],
            dancer->schmidt_triggers[dancer->write_pin_i]
        );
    }
    dancer->len_line += sprintf(
        dancer->line_buffer + dancer->len_line,
        "%.5e,%.5e,%.5e,%.5e",  // kick integral, kick schmidt, snare integral, snare schmidt
        get_next_state(dancer)[dancer->num_read_pins],
        get_next_state(dancer)[dancer->num_read_pins + dancer->num_write_pins],
        get_next_state(dancer)[dancer->num_read_pins + 1],
        get_next_state(dancer)[dancer->num_read_pins + dancer->num_write_pins + 1]
    );

    fprintf(
        dancer->outfile,
        "%s\n",
        dancer->line_buffer
    );

    dancer->buffer_i = dancer->next_buffer_i;
    pthread_mutex_unlock(&dancer->read_now_mutex);  
}

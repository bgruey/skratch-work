#include "dancer.h"

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

    free(dancer->pin_writer_thread_data->run_bool);
    free(dancer->pin_writer_thread_data->thread);
    free(dancer->pin_writer_thread_data);

    free(dancer);
}

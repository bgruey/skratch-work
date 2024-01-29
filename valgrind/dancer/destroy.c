#include "dancer.h"

void destroy_dancer(DancerState_t* dancer) {

    dancer->pin_reader_thread_data->run_bool = 0;
    dancer->pin_writer_thread_data->run_bool = 0;
    fprintf(stderr, "Sending read now cond signal pthread in dancer/destroy.c\n");
    pthread_mutex_unlock(&dancer->read_now_mutex);
    pthread_cond_signal(&dancer->read_now_cond);
    pthread_join(dancer->pin_reader_thread_data->thread, NULL);
    pthread_join(dancer->pin_writer_thread_data->thread, NULL);
    fprintf(stderr, "Threads joined\n");

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

    pthread_attr_destroy(&dancer->pthread_attr);
    pthread_mutex_destroy(&dancer->read_now_mutex);
    pthread_cond_destroy(&dancer->read_now_cond);

    free(dancer->pin_reader_thread_data);
    free(dancer->pin_writer_thread_data);

    for (i = 0; i < dancer->num_write_pins; i++)
        free(dancer->schmidt_triggers[i]);
    free(dancer->schmidt_triggers);

    free(dancer);
}

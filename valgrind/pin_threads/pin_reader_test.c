/*
    Placeholder, read data from pins (create for now)
*/
#include <stdio.h>
#include "pin_thread.h"


size_t read_sample(
    FILE* kick_file,
    FILE* snare_file, 
    double* pins,
    double time
) {
    // current time
    pins[2] = time;

    // kick
    size_t nread = fread(&pins[0], __SIZEOF_DOUBLE__, 1, kick_file);
    if (nread != 1)
        return 0;

    // snare
    nread += fread(&pins[1], __SIZEOF_DOUBLE__, 1, snare_file);
    if (nread != 2)
        return 0;
    //printf("%f: %f, %f\n", pins[0], pins[1], pins[2]);
    return nread;
}


double get_timestep(FILE* kick, FILE* snare) {
    int kick_sample_rate, snare_sameple_rate;
    fread(&kick_sample_rate, __SIZEOF_INT__, 1, kick);
    fread(&snare_sameple_rate, __SIZEOF_INT__, 1, snare);
    if( kick_sample_rate != snare_sameple_rate) {
        fprintf(
            stderr, 
            "ERROR: Sample rates mismatch in kick/snare (%d/%d)data!\n",
            kick_sample_rate,
            snare_sameple_rate
        );
        exit(EXIT_FAILURE);
    }

    double dt = 1.0 / ((double)kick_sample_rate);
    printf("%f from %d, %d\n", dt, kick_sample_rate, snare_sameple_rate);
    return dt;
}


void* pin_reader_test(void* args_in) {
    PinThreadData_t* args = (PinThreadData_t*)args_in; 

    FILE* kick_file = fopen("kick5k.dat", "rb");
    FILE* snare_file = fopen("snare5k.dat", "rb");

    args->dt = get_timestep(kick_file, snare_file);
    double t = 0.0;
    
    size_t num_read = read_sample(kick_file, snare_file, args->pins, t);

    while (args->run_bool && num_read == 2) {
        pthread_mutex_lock(args->read_now_mutex);

        while(args->read_now[0] == 0 && args->run_bool)
            pthread_cond_wait(args->read_now_cond, args->read_now_mutex);


        num_read = read_sample(
            kick_file, 
            snare_file, 
            args->pins, 
            t += args->dt
        );

        args->read_now[0] = 0;

        pthread_mutex_unlock(args->read_now_mutex);
        pthread_cond_signal(args->read_now_cond);
    }

    args->run_bool = 0;
    fclose(kick_file);
    fclose(snare_file);

    return NULL;
}

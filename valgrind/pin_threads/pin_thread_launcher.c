
#include "pin_thread.h"


int launch_pin_thread(
    pthread_attr_t* attr,
    PinThreadData_t* thread_data,
    void* (*thread_target)(void *)
) {
    int err = pthread_create(
        &thread_data->thread, 
        attr, 
        thread_target, 
        thread_data
    );

    if (err != 0)
        return err;

    return 0;
}

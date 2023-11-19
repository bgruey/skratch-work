/*
    Sleep for sleep_s seconds.

    Adapted from: https://stackoverflow.com/a/1157217
*/

#include <time.h>
#include <errno.h>    

int sleep_via_double(double sleep_s, struct timespec* ts);
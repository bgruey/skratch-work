#define _POSIX_C_SOURCE 200809L
#include <math.h>
#include <time.h>
#define TO_NS_DECIMAL 1.0e-9

#ifndef TIME_W_FLOAT_T
#define TIME_W_FLOAT_T
typedef struct TimeWFloat_t {
    struct timespec now;
    double seconds;
    double start_time_seconds;
} TimeWFloat_t;
#endif

double get_now_seconds(TimeWFloat_t* now);
double get_now_seconds_fraction(TimeWFloat_t* now, double max_window);
double get_seconds_fraction(TimeWFloat_t* now, double max_window);

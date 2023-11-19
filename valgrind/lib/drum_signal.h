/*
    Piecewise defined function:
        * A Gaussian rise from zero amplitude to 1 in the duration time
        * Then constant value for duration
        * Then an exponential decay
*/
#include <stdlib.h>
#include <math.h>

typedef struct {
    // User inputs
    double rise_duration;
    double zero_amplitude;
    double constant_duration;
    double constant_value;
    double fall_duration;

    // Calculated values
    double gaussian_rise_b;
    double exp_fall_b;
} DrumSignalData_t;


double get_drum_signal(double t, DrumSignalData_t* signal_data);

DrumSignalData_t* setup_signal_data(
    double rise_duration,
    double zero_amplitude,
    double constant_duration,
    double fall_duration
);

/*
    Piecewise defined function:
        * A Gaussian rise from zero amplitude to 1 in the duration time
        * Then constant value for duration
        * Then an exponential decay
*/
#include "drum_signal.h"
#include <stdio.h>

DrumSignalData_t* setup_signal_data(
    double rise_duration,
    double zero_amplitude,
    double constant_duration,
    double fall_duration
) {
    DrumSignalData_t* ret_data = (DrumSignalData_t*)calloc(1, sizeof(DrumSignalData_t));
    
    ret_data->rise_duration = rise_duration;
    ret_data->zero_amplitude = zero_amplitude;

    ret_data->constant_duration = constant_duration;
    ret_data->constant_value = 1.0;  // Hard-coded test value

    ret_data->fall_duration = fall_duration;

    // Calculated values
    ret_data->gaussian_rise_b = log(1.0 / ret_data->zero_amplitude) / ret_data->rise_duration / ret_data->rise_duration;
    ret_data->exp_fall_b = log(1.0 / ret_data->zero_amplitude) / ret_data->fall_duration;

    return ret_data;
}


double get_drum_signal(double t, DrumSignalData_t* signal_data) {
    double t_local = t - signal_data->rise_duration;
    if (t_local < 0.0)
        return exp(
            -signal_data->gaussian_rise_b * t_local * t_local
        );

    t_local -= signal_data->constant_duration;
    if (t_local < 0)
        return signal_data->constant_value;

    return exp(-signal_data->exp_fall_b * t_local);
}
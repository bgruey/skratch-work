#ifndef _SIGNAL_PROCESSORS_H
#define _SIGNAL_PROCESSORS_H
#include <stdio.h>
double low_pass_filter(double x_i, double y_im1, double alpha);

double shunted_integrator(double y_int, double y, double dt, double lambda);

#endif

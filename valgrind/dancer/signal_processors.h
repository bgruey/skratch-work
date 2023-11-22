#ifndef _SIGNAL_PROCESSORS_H
#define _SIGNAL_PROCESSORS_H

double low_pass_filter(double x_i, double y_im1, double alpha);

double leaky_integrator(double y, double y_int, double dt, double lambda);

#endif

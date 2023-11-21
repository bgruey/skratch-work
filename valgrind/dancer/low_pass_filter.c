#include "math.h"

double low_pass_filter(double x_i, double y_im1, double alpha) {
    return alpha * x_i + (1.0 - alpha) * y_im1;
}

#include <math.h>
#include "signal_processors.h"

double shunted_integrator(double y, double y_int, double dt, double lambda) {
    return y_int*exp(-lambda * dt) + 2 * lambda * y * dt;
}

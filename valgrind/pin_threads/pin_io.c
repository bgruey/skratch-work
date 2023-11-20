/*
    Pin I/O for direct interaction with raspberry pi pins.

    TBD.
*/
#include "pin_thread.h"


double read_pin(unsigned short pin_id, double y) {
    return (1.0 + 0.25 * pin_id) * y;
}

void write_pin(unsigned short pin_id, double y) {
    y *= y;
}
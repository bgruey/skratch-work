#include "sleep.h"


int sleep_via_double(double sleep_s, struct timespec* ts) {
    ts->tv_sec = (long)sleep_s;
    ts->tv_nsec = (long)(1.0e9 * (sleep_s - ts->tv_sec));

    if (sleep_s < 0)
    {
        errno = EINVAL;
        return -1;
    }

    int res;
    do {
        res = nanosleep(ts, ts);
    } while (res && errno == EINTR);

    return res;
}

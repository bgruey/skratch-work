#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include "lib/now.h"
#include "lib/move.h"
#include "lib/drum_signal.h"
#include "lib/sleep.h"

int main(int argc, char** argv) {
    FILE* csv_out = fopen("data.csv", "w");
    int num_points = 5000;

    TimeWFloat_t now;
    struct timespec sleep_data;

    MoveSignalState_t move_state;
    DrumSignalData_t* drum_signal_data = setup_signal_data(
        0.001, 0.01, 0.005, 0.14
    );

    int i;
    double now_s, now_fraction, drum_y, move_y;
    double start_s = get_now_seconds(&now);
    now.start_time_seconds = start_s;
    for (i = 0; i < num_points; i++) {
        now_s = get_now_seconds(&now);
        now_fraction = get_seconds_fraction(&now, 0.5);
        drum_y = get_drum_signal(
            now_fraction,
            drum_signal_data
        );
        move_y = get_move_signal(
            drum_y,
            &now,
            &move_state
        );
        // printf("%.4f  %.4f: %.4f, %.4f\n", now_s, now_fraction, drum_y, move_y);
        fprintf(csv_out, "%f,%.2e,%.2e\n", now_s, drum_y, move_y);
        sleep_via_double(0.0001, &sleep_data);
    }

    fclose(csv_out);
    return 0;
}

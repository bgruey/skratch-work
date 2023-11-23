#ifndef _SCHMIDT_TRIGGER_H
#define _SCHMIDT_TRIGGER_H

#include <stdlib.h>
#include <stdio.h>


typedef struct {
    int high; //  boolean, stores state
    double threshold_high;
    double threshold_low;
    double value_high;
    double value_low;
} SchmidtTrigger_T;


SchmidtTrigger_T* schmtt_init(
    double threshold_high,
    double threshold_low,
    double value_high,
    double value_low
);


double schmtt_calculate(double y, SchmidtTrigger_T* ts);


#endif

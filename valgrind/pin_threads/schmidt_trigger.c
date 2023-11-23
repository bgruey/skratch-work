#include "schmidt_trigger.h"


double schmtt_calculate(double y, SchmidtTrigger_T* ts) {

    if (y > ts->threshold_high) {
        ts->high = 1;
        return ts->value_high;
    }
    
    if (ts->high && y > ts->threshold_low)
        return ts->value_high;
    
    ts->high = 0;
    return ts->value_low;
}


SchmidtTrigger_T* schmtt_init(
    double threshold_high, 
    double threshold_low,
    double value_high,
    double value_low
) {
    SchmidtTrigger_T* ret = (SchmidtTrigger_T*)calloc(1, sizeof(SchmidtTrigger_T));
    
    ret->threshold_high = threshold_high;
    ret->threshold_low = threshold_low;
    
    ret->value_high = value_high;
    ret->value_low = value_low;

    return ret;
}
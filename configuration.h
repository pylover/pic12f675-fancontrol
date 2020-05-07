#ifndef H_CONFIGURATION
#define H_CONFIGURATION

#include "ntc.h"


#define DUAL_SENSOR
#define MINDUTY        10
#define SAMPLE_INTERVAL     300000

#define VCC         5
#define R2          47000
#define VOLTAGE(r)  ((VCC / (r+R2)) * R2 / VCC * 1024)

#define MAX_TEMP        800 //VOLTAGE(NTC50K_70C)
#define DEFERRED_TEMP   700 //VOLTAGE(NTC50K_50C)
#define MIN_TEMP        600 //VOLTAGE(NTC50K_35C)

#define FAN GP5

#endif

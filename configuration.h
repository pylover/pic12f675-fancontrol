#ifndef H_CONFIGURATION
#define H_CONFIGURATION

#include "ntc.h"


#define DUAL_SENSOR
#define MINDUTY        10
#define SAMPLE_INTERVAL     300000

#define VCC         5
#define R2          47000
#define VOLTAGE(r)  ((unsigned short)((VCC / (r+R2)) / VCC * R2 * 1024))

#define RISK_TEMP  VOLTAGE(NTC50K_70C)
#define HIGH_TEMP  VOLTAGE(NTC50K_50C)
#define LOW_TEMP   VOLTAGE(NTC50K_30C)


#endif

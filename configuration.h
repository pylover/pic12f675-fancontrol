#ifndef H_CONFIGURATION
#define H_CONFIGURATION

#include "ntc.h"

#define FAN GP5

//#define DUAL_SENSOR
#define SAMPLE_INTERVAL     300000

/* 
 * Minimum allowed PWM duty cycle to prevent audible nise
 */
#define MINDUTY             30

/* 
 * See ntc.h for available values 
 */
#define RISK_TEMP       NTC100K_R100K_86C
#define HIGH_TEMP       NTC100K_R100K_60C
#define LOW_TEMP        NTC100K_R100K_50C


/*
 * Power-on self test
 */
#define POST_DANCE                      5           // Count
#define POST_PWM_STEPUP_INTERVAL    50000           // Microseconds
#define POST_FULLSPEED_DURATION         2           // Seconds

#endif

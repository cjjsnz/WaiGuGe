#ifndef _EMG_ADC
#define _EMG_ADC
#ifndef RPI
#define RPI

#ifndef USE_BCM2835_LIB
#define USE_BCM2835_LIB

#include <stdlib.h>     //exit()
#include <signal.h>     //signal()
#include <time.h>
#include "ADS1263.h"
#include "stdio.h"
#include <string.h>
#ifdef __cplusplus
extern "C"{
#endif
void  Handler(int signo);
int emg_adc_convert(void);
#ifdef __cplusplus
}
#endif
#endif
#endif
#endif
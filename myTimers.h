/*
 * MyTimer.h
 *
 * Created: 11.02.2016 20:20:03
 *  Author: Christof
 */


#ifndef MYTIMER_H_
#define MYTIMER_H_

#include "timer.h"
#include "External.h"

#define MYTIMER_NUM	2

enum{SEND_REPEAT,TIMER_TIMEOUT};


void nextSend(uint8_t test);
void LED_toggle(uint8_t test);

extern volatile TIMER MyTimers[MYTIMER_NUM];
#endif /* MYTIMER_H_ */

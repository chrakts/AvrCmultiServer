/*
 * MyTimer.cpp
 *
 * Created: 11.02.2016 20:19:03
 *  Author: Christof
 */

#include "timer.h"
#include "External.h"
#include "AvrCmultiServer.h"

extern volatile uint8_t do_sleep;


// Timerwert 16 entspricht 100ms
volatile TIMER MyTimers[MYTIMER_NUM]= {	{TM_START,RESTART_YES,4,0,nextSend},
{TM_STOP,RESTART_NO,100,0,NULL}		// Timeout-Timer
};


void no_function( void )
{
	;
}

// NOTHING_TODO ->  START_CONVERSION -> WAIT_CONVERSION -> GET_TEMPERATURE -> NOTHING_TODO
void nextSend(uint8_t test)
{
	nextSendReady = true;
}

void LED_toggle(uint8_t test)
{
//	LED_KLINGEL_TOGGLE;
}


/*
 * MyTimer.cpp 
 *
 * Created: 11.02.2016 20:19:03
 *  Author: Christof
 */ 

#include "cmulti.h"

extern volatile uint8_t do_sleep;

void init_mytimer(void)
{
	CLK.RTCCTRL = CLK_RTCSRC_RCOSC_gc | CLK_RTCEN_bm;
	
	do {
		;/* Wait until RTC is not busy. */
	} while ( RTC.STATUS & RTC_SYNCBUSY_bm );

	RTC.PER = 2;	// Timerwert 16 entspricht 100ms
	RTC.CNT = 0;
	RTC.COMP = 0; 
	RTC.CTRL = 2; // Teiler 1 ???
	RTC.INTCTRL	= RTC_OVFINTLVL_MED_gc;
}

void no_function( void )
{
	;
}

ISR ( RTC_OVF_vect )
{
uint8_t i;
//	LED_KLINGEL_TOGGLE;
	PORTD_OUTTGL = PIN7_bm;
	for (i=0;i<MYTIMER_NUM;i++)
	{
		switch(MyTimers[i].state )
		{
			case TM_STOP:
			break;
			case TM_RUN:
				if (MyTimers[i].actual==0)
				{			
					if (MyTimers[i].restart == RESTART_YES)
					{
						MyTimers[i].actual = MyTimers[i].value;
						MyTimers[i].state = TM_RUN;
					}
					else
						MyTimers[i].state = TM_STOP;
					if (MyTimers[i].OnReady!=NULL)
						MyTimers[i].OnReady(3);
				}
				else
					MyTimers[i].actual--;
			break;			
			case TM_START:
				MyTimers[i].actual = MyTimers[i].value;
				MyTimers[i].state = TM_RUN;
			break;

			case TM_RESET:
				MyTimers[i].state = TM_START;
			break;
		}
	}
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


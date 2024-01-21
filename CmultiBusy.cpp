/*
 * CmultiBusy.cpp
 *
 * Created: 24.05.2018 13:30:48
 *  Author: chrak_2
 */

#include "AvrCmultiServer.h"

/* Hier wird der Pin konfiguriert, der die Interruptueberwachung bekommt, ob der Bus belegt ist */
void initReadMonitor()
{
	PORTD_INTCTRL  = PORT_INT0LVL0_bm ; // Low-Level interrupt 0 for PORTD
	PORTD_INT0MASK = PIN2_bm;
	PORTD_PIN2CTRL = PORT_ISC_BOTHEDGES_gc | PORT_OPC_PULLUP_gc ;
}

ISR(PORTD_INT0_vect)
{
	sendFree = false;
	TCC0.CNTL = 42;
}

void initBusyCounter()
{
	TCC2.CTRLE = TC2_BYTEM_SPLITMODE_gc;
	TCC2.CTRLA = TC2_CLKSEL_DIV256_gc;
	TCC2.CTRLB = 0;
	TCC2.INTCTRLA = TC2_LUNFINTLVL_LO_gc;
	TCC2.LCNT = 128; // 128
	TCC2.LPER = 42;
}

ISR ( TCC2_LUNF_vect )
{
	sendFree = true;
}

/*
 * External.h
 *
 * Created: 03.04.2017 21:04:41
 *  Author: Christof
 */ 



#ifndef EXTERNAL_H_
#define EXTERNAL_H_

#include "MyTimer.h"
#include "CRC_Calc.h"
#include "cmulti.h"

//#include "Serial.h"
//#include "Communication.h"

extern volatile uint8_t UART0_ring_received;
extern volatile  char UART0_ring_buffer[UART0_RING_BUFFER_SIZE];
extern volatile uint8_t UART1_ring_received;
extern volatile  char UART1_ring_buffer[UART1_RING_BUFFER_SIZE];
extern volatile  uint8_t sendFree; 
extern volatile TIMER MyTimers[MYTIMER_NUM];
extern volatile bool nextSendReady;

/*  */
extern Serial debug;


#endif /* EXTERNAL_H_ */
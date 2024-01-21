/*
 * Globals.cpp
 *
 * Created: 19.03.2017 09:24:37
 *  Author: Christof
 */
#define EXTERNALS_H_

#include "AvrCmultiServer.h"

char Compilation_Date[] = __DATE__;
char Compilation_Time[] = __TIME__;

volatile uint8_t UART0_ring_received;
volatile char UART0_ring_buffer[UART0_RING_BUFFER_SIZE];
volatile uint8_t UART1_ring_received;
volatile char UART1_ring_buffer[UART1_RING_BUFFER_SIZE];

volatile uint8_t sendFree;
volatile bool nextSendReady=false;

Serial debug(0);
Communication cmulti(1,"DF",8);

CRC_Calc crcGlobal;

/*
 * TLog.h
 *
 * Created: 03.04.2017 21:12:07
 *  Author: Christof
 */


#ifndef CMULTI_H_
#define CMULTI_H_
#include <avr/io.h>
#include <stdbool.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <errno.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <locale.h>


//enum{QUARZ,CLK2M,CLK32M};

#define SYSCLK CLK32M

#define PLL 0

#define  NODE 'C'
#define BROADCAST "BR"

#define UART0_RING_BUFFER_SIZE 36
#define UART1_RING_BUFFER_SIZE 36


#include "Serial.h"
#include "Communication.h"
#include "CRC_Calc.h"
#include "myTimers.h"
#include "External.h"
#include "CmultiBusy.h"
#include "watchdog.h"
#include "xmegaClocks.h"
#include "timer.h"
#include "myTimers.h"

#define LED_ROT_PIN         PIN4_bm
#define LED_ROT_OFF		    PORTA_OUTCLR = LED_ROT_PIN
#define LED_ROT_ON		    PORTA_OUTSET = LED_ROT_PIN
#define LED_ROT_TOGGLE	    PORTA_OUTTGL = LED_ROT_PIN

#define LED_GRUEN_PIN       PIN5_bm
#define LED_GRUEN_OFF		PORTA_OUTCLR = LED_GRUEN_PIN
#define LED_GRUEN_ON		PORTA_OUTSET = LED_GRUEN_PIN
#define LED_GRUEN_TOGGLE	PORTA_OUTTGL = LED_GRUEN_PIN

#define WDT_SHORT		WDT_PER_1KCLK_gc
#define WDT_LONG		WDT_PER_8KCLK_gc


#endif /* CMULTI_H_ */

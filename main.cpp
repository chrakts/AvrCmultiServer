/*
 * cmulti.cpp
 *
 * Created: 01.05.2018 14:37:19
 * Author : chrak_2
 */

#include "cmulti.h"


#define LED_PIN				PIN7_bm
#define TxD_PIN				PIN3_bm
#define RxD_PIN				PIN2_bm
#define TE_PIN				PIN0_bm
#define RE_PIN				PIN1_bm



void init_clock(int sysclk, int pll);
extern Communication cmulti;

enum{WAITSTARTBYTE=0,WAITENDBYTE1,WAITENDBYTE2,SENDCOMMAND};

int main(void)
{
    char newChar;
    char newCommand[UART0_RING_BUFFER_SIZE];
    uint8_t actPosition = 0;
    uint8_t actStatus = WAITSTARTBYTE;
    bool    actCharStatus=false;
    WDT_Reset();
	PORTC_DIRSET = TxD_PIN;
	PORTD_DIRSET = TxD_PIN|TE_PIN|RE_PIN;
	PORTD_DIRCLR = RxD_PIN;
	PORTA_DIRSET = LED_ROT_PIN | LED_GRUEN_PIN;
	PORTA_OUTCLR = LED_GRUEN_PIN | LED_ROT_PIN;
	init_clock(SYSCLK,PLL);
	initReadMonitor();
	initBusyCounter();
	init_mytimer();
	sendFree = true;
	PMIC_CTRL = PMIC_LOLVLEX_bm | PMIC_HILVLEN_bm | PMIC_MEDLVLEN_bm;
	sei();
	debug.open(Serial::BAUD_115200,F_CPU);
	cmulti.open(Serial::BAUD_57600,F_CPU);
	debug.println("->  Hello here is CmultiServer  <-");
	debug.input_flush();
	cmulti.sendInfo("CMulti hat gesendet",BROADCAST);
	cmulti.input_flush();
	WDT_EnableAndSetTimeout(WDT_LONG);
    while (1)
    {
        WDT_Reset();
		if(sendFree == true)
		{
			LED_GRUEN_ON;
		}
		else
			LED_GRUEN_OFF;
        if(debug.getChar(newChar)==true)
        {
            switch(actStatus)
            {
                case WAITSTARTBYTE:
                    //debug.println("WaitStart");
                    if(newChar=='#')
                    {
                        //debug.println("# found");
                        actStatus = WAITENDBYTE1;
                        actPosition = 0;
                        actCharStatus = true;
                        LED_ROT_ON;
                    }
                    else
                        actCharStatus = false;
                break;
                case WAITENDBYTE1:
                    //debug.println("WaitEnd1");
                    actCharStatus = true;
                    if(newChar=='\r')
                        actStatus = WAITENDBYTE2;
               break;
                case WAITENDBYTE2:
                    //debug.println("WaitEnd2");
                    actCharStatus = true;
                    if(newChar=='\n')
                        actStatus = SENDCOMMAND;
                    else
                    {
                        actStatus = WAITSTARTBYTE;
                        actPosition = 0;
                    }
                break;
            }
            if(actCharStatus==true)
            {
                newCommand[actPosition] = newChar;
                actPosition++;
                if(actPosition==UART0_RING_BUFFER_SIZE-1)
                    actPosition=0;
                newCommand[actPosition] = 0; // String-Ende-Kennung
            }
        }
        if(actStatus==SENDCOMMAND)
        {
            actStatus=WAITSTARTBYTE;
            cmulti.print(newCommand);
            debug.print("Gesendet:");
            debug.println(newCommand);
        }
    }
}

void init_clock(int sysclk, int pll)
{
	CLK_t *mein_clock;
	OSC_t *mein_osc;
	mein_clock = &CLK;
	mein_osc = &OSC;
	switch(sysclk)
	{
		case QUARZ:
		mein_osc->XOSCCTRL = OSC_FRQRANGE_12TO16_gc | OSC_XOSCSEL_XTAL_16KCLK_gc;
		//			mein_osc->XOSCCTRL = OSC_FRQRANGE_12TO16_gc | OSC_XOSCPWR_bm | OSC_XOSCSEL_XTAL_16KCLK_gc;
		mein_osc->CTRL = OSC_XOSCEN_bm | OSC_RC2MEN_bm | OSC_RC32KEN_bm; // schaltet die 32 MHz-Clock ein

		while((mein_osc->STATUS & OSC_XOSCRDY_bm) == 0)			// wartet bis diese stabil
		;
		while((mein_osc->STATUS & OSC_RC32KRDY_bm) == 0)		// wartet bis diese stabil
		;

		if ( (pll>0) & (pll<16) )
		{
			mein_osc->PLLCTRL = OSC_PLLSRC_XOSC_gc | pll;
			mein_osc->CTRL = OSC_PLLEN_bm | OSC_XOSCEN_bm | OSC_RC2MEN_bm | OSC_RC32KEN_bm; // schaltet zusätzlich die PLL ein

			while((mein_osc->STATUS & OSC_PLLRDY_bm) == 0)		// wartet bis diese stabil
			;
			CCP = CCP_IOREG_gc;										// geschuetztes Register freigeben
			mein_clock->CTRL = CLK_SCLKSEL_PLL_gc;					// umschalten auf PLL-Clock
			mein_osc->CTRL = OSC_PLLEN_bm | OSC_XOSCEN_bm | OSC_RC32KEN_bm;
		}
		else
		{
			CCP = CCP_IOREG_gc;										// geschuetztes Register freigeben
			mein_clock->CTRL = CLK_SCLKSEL_XOSC_gc;					// umschalten auf XOSC-Clock
			mein_osc->CTRL = OSC_XOSCEN_bm | OSC_RC32KEN_bm;
		}
		break; // QUARZ
		case CLK2M:
            mein_osc->CTRL = OSC_RC2MEN_bm | OSC_RC32KEN_bm; // schaltet die 2 MHz-Clock ein
            while((mein_osc->STATUS & OSC_RC2MRDY_bm) == 0)  // wartet bis diese stabil
            ;
            while((mein_osc->STATUS & OSC_RC32KRDY_bm) == 0)  // wartet bis diese stabil
            ;
            CCP = CCP_IOREG_gc;								// geschuetztes Register freigeben
            mein_clock->CTRL = CLK_SCLKSEL_RC2M_gc;		// umschalten auf 2 MHz-Clock
            //			CLKSYS_AutoCalibration_Enable(OSC_RC2MCREF_RC32K_gc,false); // OSC_RC32MCREF_bm
		break;
		case CLK32M:
            mein_osc->CTRL = OSC_RC32MEN_bm | OSC_RC2MEN_bm | OSC_RC32KEN_bm; // schaltet die 32 MHz-Clock ein
            while((mein_osc->STATUS & OSC_RC32MRDY_bm) == 0)  // wartet bis diese stabil
            ;
            while((mein_osc->STATUS & OSC_RC32KRDY_bm) == 0)  // wartet bis diese stabil
            ;
            CCP = CCP_IOREG_gc;								// geschuetztes Register freigeben
            mein_clock->CTRL = CLK_SCLKSEL_RC32M_gc;		// umschalten auf 32 MHz-Clock
            mein_osc->CTRL = OSC_RC32MEN_bm | OSC_RC32KEN_bm;		// abschalten der 2 MHz-Clock
            //			CLKSYS_AutoCalibration_Enable(OSC_RC32MCREF_RC32K_gc,false); // OSC_RC32MCREF_bm
		break;
	}
}

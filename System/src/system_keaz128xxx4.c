/*
** ###################################################################
**     Compilers:           ARM Compiler
**                          Freescale C/C++ for Embedded ARM
**                          GNU C Compiler
**                          GNU C Compiler - CodeSourcery Sourcery G++
**                          IAR ANSI C/C++ Compiler for ARM
**
**     Reference manual:    MKE06P80M48SF0RM, Rev. 1, Dec 2013
**     Version:             rev. 1.2, 2014-01-10
**
**     Abstract:
**         Provides a system configuration function and a global variable that
**         contains the system frequency. It configures the device and initializes
**         the oscillator (PLL) that is part of the microcontroller device.
**
**     Copyright: 2014 Freescale, Inc. All Rights Reserved.
**
**     http:                 www.freescale.com
**     mail:                 support@freescale.com
**
**     Revisions:
**     - rev. 1.0 (2013-07-30)
**         Initial version.
**     - rev. 1.1 (2013-10-29)
**         Definition of BITBAND macros updated to support peripherals with 32-bit acces disabled.
**     - rev. 1.2 (2014-01-10)
**         CAN module: corrected address of TSIDR1 register.
**         CAN module: corrected name of MSCAN_TDLR bit DLC to TDLC.
**         FTM0 module: added access macro for EXTTRIG register.
**         NVIC module: registers access macros improved.
**         SCB module: unused bits removed, mask, shift macros improved.
**         Defines of interrupt vectors aligned to RM.
**
** ###################################################################
*/

/*!
* @file SKEAZ1284
* @version 1.2
* @date 2014-01-10
* @brief Device specific configuration file for SKEAZ1284 (implementation file)
*
* Provides a system configuration function and a global variable that contains
* the system frequency. It configures the device and initializes the oscillator
* (PLL) that is part of the microcontroller device.
*/

#include <stdint.h>
#include "KinetisKE_Common.h"

#define DISABLE_WDOG    1

/*----------------------------------------------------------------------------
Define clock source values
*----------------------------------------------------------------------------*/
#ifndef CPU_XTAL_CLK_HZ 
 #define CPU_XTAL_CLK_HZ                 8000000u /* Value of the external crystal or oscillator clock frequency in Hz */
#endif /*CPU_XTAL_CLK_HZ*/

#define CPU_INT_CLK_HZ                  37500u   /* Value of the internal oscillator clock frequency in Hz  */
#define CPU_INT_LPO_HZ					1000u	/* Value of the internal low-power oscillator clock frequency in Hz */
/* ----------------------------------------------------------------------------
-- Core clock & Bus Clock
---------------------------------------------------------------------------- */

uint32_t SystemCoreClock = 0;
uint32_t SystemBusClock = 0;

/* ----------------------------------------------------------------------------
-- SystemInit()
---------------------------------------------------------------------------- */

void SystemInit(void) 
{	
#if (DISABLE_WDOG)
	/* WDOG->TOVAL: TOVAL=0xE803 */
	WDOG->TOVAL = WDOG_TOVAL_TOVAL(0xE803); /* Timeout value */
	/* WDOG->CS2: WIN=0,FLG=0,??=0,PRES=0,??=0,??=0,CLK=1 */
	WDOG->CS2 = WDOG_CS2_CLK(0x01);       /* 1-kHz clock source */
	/* WDOG->CS1: EN=0,INT=0,UPDATE=1,TST=0,DBG=0,WAIT=1,STOP=1 */
	WDOG->CS1 = WDOG_CS1_UPDATE_MASK |
		WDOG_CS1_TST(0x00) |
			WDOG_CS1_WAIT_MASK |
				WDOG_CS1_STOP_MASK;
	
#endif /* (DISABLE_WDOG) */
	/* ICS->C2: BDIV|=1 */
	ICS->C2 |= ICS_C2_BDIV(0x01);         /* Update system prescalers */
	/* SIM->CLKDIV: ??=0,??=0,OUTDIV1=0,??=0,??=0,??=0,OUTDIV2=0,??=0,??=0,??=0,OUTDIV3=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0,??=0 */
	SIM->CLKDIV = SIM_CLKDIV_OUTDIV1(0x00); /* Update system prescalers */
	/* Switch to FEI Mode */
	/* ICS_C1: CLKS=0,RDIV=0,IREFS=1,IRCLKEN=1,IREFSTEN=0 */
	ICS->C1 = ICS_C1_CLKS(0x00) |
		ICS_C1_RDIV(0x00) |
			ICS_C1_IREFS_MASK |
				ICS_C1_IRCLKEN_MASK;
	/* ICS->C2: BDIV=1,LP=0 */
	ICS->C2 = (uint8_t)((ICS->C2 & (uint8_t)~(uint8_t)(
													   ICS_C2_BDIV(0x06) |
														   ICS_C2_LP_MASK
															   )) | (uint8_t)(
																			  ICS_C2_BDIV(0x01)
																				  ));
	/* OSC->CR: OSCEN=0,??=0,OSCSTEN=0,OSCOS=0,??=0,RANGE=0,HGO=0,OSCINIT=0 */
	OSC->CR = 0x00U;
	while((ICS->S & ICS_S_IREFST_MASK) == 0x00U) 
	{ 
		/* Check that the source of the FLL reference clock is the internal reference clock. */
	}
	while((ICS->S & 0x0CU) != 0x00U) 
	{    
		/* Wait until output of the FLL is selected */
	}

    //Update Clock Frequency
    SystemClock_Update();
}

/* ----------------------------------------------------------------------------
-- SystemCoreClockUpdate()
---------------------------------------------------------------------------- */

//void SystemClock_Update(void) 
//{
//	
//	uint32_t ICSOUTClock;                                                        /* Variable to store output clock frequency of the ICS module */
//	uint8_t Divider;
//	
//	if ((ICS->C1 & ICS_C1_CLKS_MASK) == 0x0u) {
//		/* Output of FLL is selected */
//		if ((ICS->C1 & ICS_C1_IREFS_MASK) == 0x0u) {
//			/* External reference clock is selected */
//			ICSOUTClock = CPU_XTAL_CLK_HZ;                                         /* System oscillator drives ICS clock */
//			Divider = (uint8_t)(1u << ((ICS->C1 & ICS_C1_RDIV_MASK) >> ICS_C1_RDIV_SHIFT));
//			ICSOUTClock = (ICSOUTClock / Divider);  /* Calculate the divided FLL reference clock */
//			if ((OSC->CR & OSC_CR_RANGE_MASK) != 0x0u) {
//				ICSOUTClock /= 32u;                                                  /* If high range is enabled, additional 32 divider is active */
//			}
//		} else {
//			ICSOUTClock = CPU_INT_CLK_HZ;                                          /* The internal reference clock is selected */
//		}
//		ICSOUTClock *= 1280u;                                                    /* Apply 1280 FLL multiplier */
//	} else if ((ICS->C1 & ICS_C1_CLKS_MASK) == 0x40u) {
//		/* Internal reference clock is selected */
//		ICSOUTClock = CPU_INT_CLK_HZ;
//	} else if ((ICS->C1 & ICS_C1_CLKS_MASK) == 0x80u) {
//		/* External reference clock is selected */
//		ICSOUTClock = CPU_XTAL_CLK_HZ;
//	} else {
//		/* Reserved value */
//		return;
//	}
//	ICSOUTClock = ICSOUTClock >> ((ICS->C2 & ICS_C2_BDIV_MASK) >> ICS_C2_BDIV_SHIFT);
//	SystemCoreClock = (ICSOUTClock / (1u + ((SIM->CLKDIV & SIM_CLKDIV_OUTDIV1_MASK) >> SIM_CLKDIV_OUTDIV1_SHIFT)));
//	if((SIM->CLKDIV & SIM_CLKDIV_OUTDIV1_MASK) >> SIM_CLKDIV_OUTDIV2_SHIFT == 1)
//	{
//		SystemBusClock = SystemCoreClock / 2;
//	}
//	else 
//	{
//		SystemBusClock = SystemCoreClock;
//	}
//}

void SystemClock_Update(void)
{
	//ICSOUTCLK & Divider
	uint32_t ics_clk = 0;
	uint16_t div;
	
	/* Calculate for ICSOUTCLK*/
	//ICS is in FEE or FEI mode
	if((ICS->C1 & ICS_C1_CLKS_MASK) == 0x00u)
	{
		if((ICS->C1 & ICS_C1_IREFS_MASK) == 0x00u)
		{
			//external reference clock is selected
			ics_clk = CPU_XTAL_CLK_HZ;
			div = 1 << ((ICS->C1 & ICS_C1_RDIV_MASK) >> ICS_C1_RDIV_SHIFT);
			ics_clk = ics_clk / div;
			if((OSC->CR & OSC_CR_RANGE_MASK) == 0x04u)
			{
				//external reference is High Range
				ics_clk /= 32u;
			}
		}
		else if((ICS->C1 & ICS_C1_IREFS_MASK) == 0x04u)
		{
			//external reference clock is selected
			ics_clk = CPU_INT_CLK_HZ;
		}
		
		//FLL Gain is 1280u
		ics_clk *= 1280u;
	}
	//ICS is in FBI/FBILP mode
	else if((ICS->C1 & ICS_C1_CLKS_MASK) == 0x40u)
	{
		ics_clk = CPU_INT_CLK_HZ;
	}
	//ICS is in FBE/FBELP mode
	else if((ICS->C1 & ICS_C1_CLKS_MASK) == 0x80u)
	{
		ics_clk = CPU_XTAL_CLK_HZ;
	}
	//Error
	else return;
	
	ics_clk >>= ((ICS->C2 & ICS_C2_BDIV_MASK) >> ICS_C2_BDIV_SHIFT);
	
	/* Calculate for SystemClock Frequency */
	SystemCoreClock = ics_clk / (1u + ((SIM->CLKDIV & SIM_CLKDIV_OUTDIV1_MASK) >> SIM_CLKDIV_OUTDIV1_SHIFT));
	if((SIM->CLKDIV & SIM_CLKDIV_OUTDIV2_MASK) != 0x00u)
	{
		SystemBusClock = SystemCoreClock / 2;
	}
	else 
	{
		SystemBusClock = SystemCoreClock;
	}
	
}

//EOF

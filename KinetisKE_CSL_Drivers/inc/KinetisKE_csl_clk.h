/**
 * Title 	Clock module in CSL for KEAZ128(Header File)
 * License	GPLv2.0
 * Author	Stark Zhang
 * Debug	None
**/

/* This is for ICS & OCR modules */
/* There are all of Clock configurations of KinetisKE MCUs */

#ifndef __KinetisKE_CSL_CLK_H
#define __KinetisKE_CSL_CLK_H

#ifdef __cplusplus
 extern "C" {
#endif /*__cplusplus*/

#include "KinetisKE_csl.h"

/**
 * enum of external clock signals
**/
typedef enum
{
	OSC_BYPASS 		= 0x00u,
	OSC_CRYSTAL		= 0x10u,
}OSC_SourceTypeDef;

/**
 * defination of CLK structure
**/
typedef struct
{
	//ICS & FLL settings
	uint8_t SystemClkSource;				//Core Clock Source
	uint8_t FLLClkSource;					//FLL Clock Source 
	uint8_t RDIV;							//Reference Clock Frequency Scaler
	uint8_t BDIV;							//Bus Clock Scaler
	
	//OSC Settings
	OSC_SourceTypeDef Type;					//External clock signals
	uint8_t Range;							//Ext-CLKsource Frequency
	uint8_t PowerMode;						//LP or HF
	
	//SIM->CLKDIV
	uint32_t Core_PreScaler;					//Core ClkFrequency after FLL DIV
	uint32_t Bus_PreScaler;					//Bus/Flash ClkFrequency after FLL DIV
}CLK_ConfigTypeDef;

/* ICS Registers Defination */
/**
 * System Clock source 
**/
#define ICS_CLKSource_FLL       0x00u         //FLL Clock OUT
#define ICS_CLKSource_IRC       0x40u         //Internal Reference Clock
#define ICS_CLKSource_ERC       0x80u         //External Reference Clock

/**
 * Reference Clock Frequency Scaler
 * if OSC_CR[RANGE] = 0, the DIV = 2^n, else DIV = 2^(n*5)
**/
#define ICS_RDIV_DIV0           0x00u             //DIV = 1 or DIV = 32(OSC_CR[RANGE] = 1)
#define ICS_RDIV_DIV1           0x08u             //DIV = 2 or DIV = 64(OSC_CR[RANGE] = 1)
#define ICS_RDIV_DIV2           0x10u             //DIV = 4 or DIV = 128(OSC_CR[RANGE] = 1)
#define ICS_RDIV_DIV3           0x18u             //DIV = 8 or DIV = 256(OSC_CR[RANGE] = 1)
#define ICS_RDIV_DIV4           0x20u             //DIV = 16 or DIV = 512(OSC_CR[RANGE] = 1)
#define ICS_RDIV_DIV5           0x28u             //DIV = 32 or DIV = 1024(OSC_CR[RANGE] = 1)
#define ICS_RDIV_DIV6           0x30u             //DIV = 64 or DIV is reserved(OSC_CR[RANGE] = 1)
#define ICS_RDIV_DIV7           0x38u             //DIV = 128 or DIV is reserved(OSC_CR[RANGE] = 1)

/**
 * Internal Reference Clock Selection
**/
#define ICS_FLL_ERC           	0x00u             //External Reference Clock
#define ICS_FLL_IRC          	0x04u             //Internal Reference Clock

/**
 * Bus Clock Scaler --> DIV = 2^n
**/
#define ICS_BDIV_DIV0           0x00u             	//DIV = 1
#define ICS_BDIV_DIV1           0x20u             	//DIV = 2
#define ICS_BDIV_DIV2			0x40u				//DIV = 4
#define ICS_BDIV_DIV3			0x60u				//DIV = 8
#define ICS_BDIV_DIV4			0x80u				//DIV = 16
#define ICS_BDIV_DIV5			0xA0u				//DIV = 32
#define ICS_BDIV_DIV6			0xC0u				//DIV = 64
#define ICS_BDIV_DIV7			0xE0u				//DIV = 128

/* Note: This is not support to ICS_C3 */

/**
 * Clock Monitoring
**/
#define ICS_CME_DISABLE         0x00u         	//Disable Clock Monitoring
#define ICS_CME_ENABLE          0x20u         	//Enable Clock Monitoring

/* OCR Registers Defination */

/**
 * OSC mode Selection
**/
#define OSC_OS_BYPASS           0x00u        	//Clock source from EXTAL pin
#define OSC_OS_CRAYSTAL         0x10u       	//Clock source from XTAL/EXTAL pins

/**
 * Range of OSC Frequency
**/
#define OSC_RANGE_LF            0x00u         	//Low-Frequency 32kHz
#define OSC_RANGE_HF            0x04u         	//High-Frequency 4~24MHz

/**
 * High gain OSC
**/
#define OSC_HGO_HG              0x02u         	//High-Gain mode
#define OSC_HGO_LP              0x00u         	//Low-Power mode

/**
 * Core Clock Frequency Divider
**/
#define Core_CLK_DIV0			0x00000000u			//DIV = 1
#define Core_CLK_DIV1			0x10000000u			//DIV = 2
#define Core_CLK_DIV2			0x20000000u			//DIV = 4
#define Core_CLK_DIV3			0x30000000u			//DIV = 8

/**
 * Bus/Flash Clock Frequency Divider
**/
#define Bus_CLK_DIV0			0x00000000u			//DIV = 1
#define Bus_CLK_DIV1			0x01000000u			//DIV = 2

/**
 * Bus/Flash Clock Output PreScaler
**/
#define CLK_BUSREF_1			0x00000000u			//DIV = 1
#define CLK_BUSREF_2			0x00010000u
#define CLK_BUSREF_4			0x00020000u
#define CLK_BUSREF_8			0x00030000u
#define CLK_BUSREF_16			0x00040000u
#define CLK_BUSREF_32			0x00050000u
#define CLK_BUSREF_64			0x00060000u
#define CSL_BUSREF_128			0x00070000u

/* Macors Functions */

/**
 * @brief	Enable/Disable ICS Interrupt
**/
#define __ICS_ENABLE_EXTI()						(ICS->C4 |= ICS_C4_LOLIE_MASK)
#define __ICS_DISABLE_EXTI()					(ICS->C4 &= ~ICS_C4_LOLIE_MASK)

/**
 * @brief	Enable/Disable Clock Minitor(CME)
**/
#define __ICS_ENABLE_CME()						(ICS->C4 |= ICS_C4_CME_MASK)
#define __ICS_DISABLE_CME()						(ICS->C4 &= ~ICS_C4_CME_MASK)

/**
 * @brief	Clear FLL Loss of Lock Status Flag
**/
#define __ICS_FLL_CLEAR_FLAG()					(ICS->S |= ICS_S_LOLS_MASK)

/**
 * @brief 	Get ICS or OSC State
**/
#define __CSL_ICS_GetState(__FLAG__)			((ICS->S & __FLAG__) ? SET : RESET)
#define __CSL_OSC_GetState(__FLAG__)			((OSC->CR & __FLAG__) ? SET : RESET)

/* Functions of Clock Module */
CSL_StatusTypeDef CSL_CLK_ClockSourceConfig(CLK_ConfigTypeDef* CLK_InitStructure);
void CSL_CLK_MspConfig(CLK_ConfigTypeDef* CLK_ConfigStructure);
void CSL_CLK_TIMClockSouceConfig(uint8_t TimerDivider);
void CSL_CLK_BusClockOutputEnable(uint32_t busref);
void CSL_CLK_BusClockOutputDisable(void);
uint32_t CSL_CLK_GetCoreFrequency(void);
uint32_t CSL_CLK_GetBusFrequency(void);
uint32_t CSL_CLK_GetTIMFrequency(void);

/* Interrupt functions */
void CSL_ICS_IRQHandler(void);
void CSL_ICS_Callback(void);

/* Defgroup CLOCK_Private_Macros CORTEX Private Macros */
#define IS_CLK_SYSCLK(x)		(((x) == ICS_CLKSource_FLL) || \
								 ((x) == ICS_CLKSource_IRC) || \
								 ((x) == ICS_CLKSource_ERC))

#define IS_CLK_RDIV(x)			(((x) == ICS_RDIV_DIV0) || \
								 ((x) == ICS_RDIV_DIV1) || \
								 ((x) == ICS_RDIV_DIV2) || \
								 ((x) == ICS_RDIV_DIV3) || \
								 ((x) == ICS_RDIV_DIV4) || \
								 ((x) == ICS_RDIV_DIV5) || \
								 ((x) == ICS_RDIV_DIV6) || \
								 ((x) == ICS_RDIV_DIV7))

#define IS_CLK_BDIV(x)			(((x) == ICS_BDIV_DIV0) || \
								 ((x) == ICS_BDIV_DIV1) || \
								 ((x) == ICS_BDIV_DIV2) || \
								 ((x) == ICS_BDIV_DIV3) || \
								 ((x) == ICS_BDIV_DIV4) || \
								 ((x) == ICS_BDIV_DIV5) || \
								 ((x) == ICS_BDIV_DIV6) || \
								 ((x) == ICS_BDIV_DIV7))

#define IS_OSC_OS(x)			(((x) == OSC_BYPASS) || \
								 ((x) == OSC_CRYSTAL))

#define IS_OSC_RANGE(x)			(((x) == OSC_RANGE_LF) || \
								 ((x) == OSC_RANGE_HF))

#define IS_OSC_PowerMODE(x)		(((x) == OSC_HGO_HG) || \
								 ((x) == OSC_HGO_LP))

#define IS_Core_PSC(x)			(((x) == Core_CLK_DIV0) || \
								 ((x) == Core_CLK_DIV1) || \
								 ((x) == Core_CLK_DIV2) || \
								 ((x) == Core_CLK_DIV3))

#define IS_Bus_PSC(x)			(((x) == Bus_CLK_DIV0) || \
								 ((x) == Bus_CLK_DIV1))
#define IS_CLK_BUSREF(x)	(((x+1) >= 0x01u) && (x <= 0x07u))

#ifdef __cplusplus
 }
#endif /*__cplusplus*/

#endif /*__KinetisKE_CSL_CLK_H*/

//EOF

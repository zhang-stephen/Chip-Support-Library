/**
 * Title 	PWT module in CSL for KEAZ128(Header File)
 * License	GPLv2.0
 * Author	Stark Zhang
 * Debug	None
**/

/* Pulse Width Timer, only for KEAZ128xxx */

#ifndef __KinetisKE_CSL_PWT_H
#define __KinetisKE_CSL_PWT_H

#ifdef __cplusplus
 extern "C" {
#endif /*__cplusplus*/

#include "KinetisKE_csl.h"

/**
 * PWT Initialize Structure
**/
typedef struct
{
	uint16_t ClockSource;			//Clock Source for PWT
	uint32_t Inx;					//PWT input 
	uint16_t Edge;					//Edge to Detect
	uint16_t PreScaler;				//Clock PreScaler
	uint16_t TCLK_Pin;				//if TCLK is selected, this is to select TCLK Pin
	uint32_t Input_Pin;				//External Input Signal Pins
}PWT_InitTypeDef;

/**
 * PWT GLobal State
**/
typedef enum
{
	CSL_PWT_STATE_RESET			= 0x00,			//Reset
	CSL_PWT_STATE_READY			= 0x01,			//Ready
	CSL_PWT_STATE_BUSY			= 0x02,			//Busy
	CSL_PWT_STATE_ORIF			= 0x03,			//OverFlow or Overrun(IT Flag)
	CSL_PWT_STATE_RIF			= 0x04,			//Ready(IT Flag)
}PWT_gStateTypeDef;

/**
 * PWT Handle Structure
**/
typedef struct
{
	PWT_InitTypeDef Init;
	CSL_LockTypeDef Lock;
	PWT_gStateTypeDef gState;
}PWT_HandleTypeDef;

/**
 * Clock Source of PWT
**/
#define PWT_CLKSource_TIMCLK		0x0000u
#define PWT_CLKSource_TCLK			PWT_R1_PCLKS_MASK

/**
 * PWT Clock Prescaler
**/
#define PWT_CLKSource_DIV1			0x0000u
#define PWT_CLKSource_DIV2			0x0100u
#define PWT_CLKSource_DIV4			0x0200u
#define PWT_CLKSource_DIV8			0x0300u
#define PWT_CLKSource_DIV16			0x0400u
#define PWT_CLKSource_DIV32			0x0500u
#define PWT_CLKSource_DIV64			0x0600u
#define PWT_CLKSource_DIV128		0x0700u


/**
 * PWT input Select
**/
#define PWT_IN_0					0x00000000u
#define PWT_IN_1					0x10000000u
#define PWT_IN_2					0x20000000u
#define PWT_IN_3					0x40000000u

/**
 * PWT external signal Pins
**/
//Ext Clock
#define PWT_ExtCLK_TCLK0			(~SIM_PINSEL_PWTCLKPS_MASK)
#define PWT_ExtCLK_TCLK1			((uint32_t)(0x40000000u)
#define PWT_ExtCLK_TCLK2			SIM_PINSEL_PWTCLKPS_MASK
//Ext Input Pin
#define PWT_IN0_PTD5				0x0000u
#define PWT_IN0_PTE2				0x4000u
#define PWT_IN1_PTB0				0x0000u
#define PWT_IN1_PTH7				0x8000u

/**
 * PWT Edge Detect
**/
#define PWT_Edge_E1					0x0000u
#define PWT_Edge_E2					0x0800u
#define PWT_Edge_E3					0x1000u
#define PWT_Edge_E4					0x1800u

/* Macros Functions */
/**
 * @brief	Enable/Disable PWT
**/
#define __PWT_ENBALE()				(SET_BIT(PWT->R1, PWT_R1_PWTEN_MASK))
#define __PWT_DISABLE()				(CLEAR_BIT(PWT->R1, PWT_R1_PWTEN_MASK))

/**
 * @brief 	Enable/Disable PWT Global Interrupt
**/
#define __PWT_GLOBAL_IT_ENABLE()	(SET_BIT(PWT->R1, PWT_R1_PWTIE_MASK))
#define __PWT_GLOBAL_IT_DISABLE()	(CLEAR_BIT(PWT->R1, PWT_R1_PWTIE_MASK))

/**
 * @brief	Enable/Disable PWT Ready Interrupt
**/
#define __PWT_READY_IT_ENABLE()		(SET_BIT(PWT->R1, PWT_R1_PRDYIE_MASK))
#define __PWT_READY_IT_DISABLE()	(CLEAR_BIT(PWT->R1, PWT_R1_PRDYIE_MASK))

/**
 * Enable/Disable PWT OverFlow Interrupt
**/
#define __PWT_OVERFLOW_IT_ENABLE()	(SET_BIT(PWT->R1, PWT_R1_POVIE_MASK))
#define __PWT_OVERFLOW_IT_DISABLE()	(CLEAR_BIT(PWT->R1, PWT_R1_POVIE_MASK))

/**
 * @brief	Clear PWT IT Flag
**/
#define __CSL_PWT_RDY_FLAGS_CLEAR()	(PWT_R1 ^= PWT_R1_PWTRDY_MASK)
#define __CSL_PWT_OV_FLAGS_CLEAR()	(PWT_R1 ^= PWT_R1_PWTOV_MASK)

/**
 * @brief	Get PWT Counter
**/
#define __PWT_GET_COUNT()			((uint16_t)(PWT->R2 >> 16))

/* Functions */
CSL_StatusTypeDef CSL_PWT_Init(PWT_HandleTypeDef* cpwt);
CSL_StatusTypeDef CSL_PWT_DeInit(PWT_HandleTypeDef* cpwt);
void CSL_PWT_MspInit(PWT_HandleTypeDef* cpwt);
void CSL_PWT_MspDeInit(PWT_HandleTypeDef* cpwt);
void CSL_PWT_SoftRST(PWT_HandleTypeDef* cpwt);
uint16_t CSL_PWT_GetPPW(void);
uint16_t CSL_PWT_GetNPW(void);

void CSL_PWT_IRQHandler(PWT_HandleTypeDef* cpwt);
void CSL_PWT_ReadyCallback(PWT_HandleTypeDef* cpwt);
void CSL_PWT_OverFlowCallback(PWT_HandleTypeDef* cpwt);

/* Defgroup of PWT_MODULE */
#define IS_PWT_CLKSource(CLK)		((CLK == PWT_CLKSource_TIMCLK) || (CLK == PWT_CLKSource_TCLK))
#define IS_PWT_CLKSourceDIV(div)	((div+1 >= 0x0100u) && (div <= 0x0700u))
#define IS_PWT_INx(in)				((in == PWT_IN_0) || \
									 (in == PWT_IN_1) || \
									 (in == PWT_IN_2) || \
									 (in == PWT_IN_3))
#define IS_PWT_EDGE(edge)			((edge == PWT_Edge_E1) || \
									 (edge == PWT_Edge_E2) || \
									 (edge == PWT_Edge_E3) || \
									 (edge == PWT_Edge_E4))


#ifdef __cplusplus
 }
#endif /*__cplusplus*/

#endif /*__KinetisKE_CSL_PWT_H*/

//EOF

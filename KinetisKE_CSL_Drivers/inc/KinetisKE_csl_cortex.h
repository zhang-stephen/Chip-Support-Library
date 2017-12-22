/**
 * Title 	Cortex-M0+ module in CSL for KEAZ128(Header File)
 * License	GPLv2.0
 * Author	Stark Zhang
 * Debug	None
**/

/* KEA is not supported to MPU & FPU, only for NVIC & Systick */

#ifndef __KinetisKE_CSL_Cortex_H
#define __KinetisKE_CSL_Cortex_H

#ifdef __cplusplus
 extern "C" {
#endif /*__cplusplus*/

#include "KinetisKE_csl.h"

/**
 * Systick Clock Source Divider
**/
#define SysTick_CLKSource_CoreCLK				0x00000004u
#define SysTick_CLKSource_CoreCLK_DIV16			0x00000000u

/* Initialization and de-initialization functions */
//NVIC
void CSL_NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority);
void CSL_NVIC_EnableIRQ(IRQn_Type IRQn);
void CSL_NVIC_DisableIRQ(IRQn_Type IRQn);
void CSL_NVIC_SystemReset(void);

//SysTick
uint32_t CSL_SYSTICK_Config(uint32_t TickNumb);
void CSL_SYSTICK_ClockSourceConfig(uint32_t TickClockSource);

//Systick Interrupt Functions
void CSL_SYSTICK_IRQHandler(void);
void CSL_SYSTICK_Callback(void);

/* NVIC Functions */
uint32_t CSL_NVIC_GetPendingIRQ(IRQn_Type IRQn);
void CSL_NVIC_SetPendingIRQ(IRQn_Type IRQn);
void CSL_NVIC_ClearPendingIRQ(IRQn_Type IRQn);

/* Defgroup CORTEX_Private_Macros CORTEX Private Macros */
#define IS_NVIC_Device_IRQn(IRQn)       (((IRQn) >= (IRQn_Type)0) && (IRQn) <=(IRQn_Type)31)
#define IS_SYSTICK_CLKSource(CLK)		((CLK == 0x04u) || (CLK == 0x00u))

#ifdef __cplusplus
 }
#endif /*__cplusplus*/

#endif /*__KinetisKE_CSL_Cortex_H*/

//EOF

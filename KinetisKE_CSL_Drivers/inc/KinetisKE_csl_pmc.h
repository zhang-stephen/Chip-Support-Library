/**
 * Title 	Power Management Controller module in CSL for KEAZ128(Header File)
 * License	GPLv2.0
 * Author	Stark Zhang
 * Debug	None
**/

#ifndef __KinetisKE_CSL_PMC_H
#define __KinetisKE_CSL_PMC_H

#ifdef __cplusplus
 extern "C" {
#endif /*__cplusplus*/

#include "KinetisKE_csl.h"

/**
 * enum of Detection Range
**/
typedef enum
{
	LVD_Range_Low	= 0,		//Low-Range 2.56V ~ 3.08V
	LVD_Range_High	= 1,		//High-Range 4.2V ~ 4.8V
}LVD_RangeTypeDef;

/**
 * Low-Voltage Detection Initialization
**/
typedef struct
{
	LVD_RangeTypeDef Range;		//Range of LVD 
	uint8_t VDL;				//Voltage Drop Level(0x00u-0x03u)
}LVD_InitTypeDef;

/**
 * PMC Handle
**/
typedef struct
{
	LVD_InitTypeDef Init;
	uint8_t LVD_State;
	uint8_t ErrorCode;
}PMC_HandleTypeDef;

/**
 * Definations of Voltage Drop Level(VDL)
**/							//Range: 		Low Range	|	High Range
#define LVD_VDrop_L0		0x00u	//	2.62 ~ 2.78V	| 	4.3 ~ 4.5V
#define LVD_VDrop_L1		0x10u	//	2.72 ~ 2.88V	|	4.5 ~ 4.6V
#define LVD_VDrop_L2		0x20u	//	2.82 ~ 2.98V	|	4.6 ~ 4.7V
#define LVD_VDrop_L3		0x30u	//	2.92 ~ 3.08V	|	4.7 ~ 4.8V

/**
 * LVD State
**/
#define CSL_LVD_STATE_NONE			0x00u		//Normally Boot
#define CSL_LVD_STATE_POR			0x01u		//Boot after POR
#define CSL_LVD_STATE_LVWPOR		0x02u		//Boot after POR which caused by LVD

/**
 * Error Code of PMC
**/
#define CSL_PMC_ERROR_NONE			0x00u		//no error
#define CSL_PMC_ERROR_CONFIG		0x01u		//Configure Failed

/**
 * Low-power mode entry
**/
#define PMC_LowPower_Entry_WFI		0x00u		//Wait for Interrupt
#define PMC_LowPower_Entry_WFE		0x01u		//Wait for Event

/** Macros Functions **/
/**
 * Enable/Disable PMC EXTI
**/
#define __PMC_LVD_EXTI_ENABLE()			(PMC->SPMSC1 |= PMC_SPMSC1_LVWIE_MASK)
#define __PMC_LVD_EXTI_DISABLE()		(PMC->SPMSC1 &= ~PMC_SPMSC1_LVWIE_MASK)

/**
 * Clear Low-Voltage Warning Flag
**/
#define __PMC_LVD_CLEAR_FLAG()			(PMC->SPMSC1 |= PMC_SPMSC1_LVWACK_MASK);

/** PMC Functions **/
CSL_StatusTypeDef CSL_PMC_LVD_Init(PMC_HandleTypeDef* cpmc);
void CSL_PMC_LVD_MspInit(void);
void CSL_PMC_LVD_MspDeInit(void);
uint8_t CSL_PMC_GetLVDState(PMC_HandleTypeDef* cpmc);
uint8_t CSL_PMC_GetError(PMC_HandleTypeDef* cpmc);

//Low-Power Mode: Stop & Wait
void CSL_PMC_EnterWaitMode(uint8_t LowPowerEntry);
void CSL_PMC_EnterSTOPMode(uint8_t LowPowerEntry);
void CSL_PMC_EnterStandByMode(uint8_t LowPowerEntry);

//Interrupt Fucntions
void CSL_PMC_IRQHandler(PMC_HandleTypeDef* cpmc);
void CSL_PMC_LVD_Callback(void);

/* Defgroup of PMC_MODULE Paramter Check */
#define IS_LVD_VDL(x)		(((x+1) >= 0x01u) && (x < LVD_VDrop_L3))
#define IS_PMC_LPEntry(x)	((x == PMC_LowPower_Entry_WFI) || (x == PMC_LowPower_Entry_WFE))

#ifdef __cplusplus
 }
#endif /*__cplusplus*/

#endif /*__KinetisKE_CSL_PMC_H*/

//EOF

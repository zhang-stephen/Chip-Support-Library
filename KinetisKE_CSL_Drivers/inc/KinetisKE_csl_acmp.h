/**
 * Title 	Analog Comparator(ACMP) module in CSL for KEAZ128(Header File)
 * License	GPLv2.0
 * Author	Stark Zhang
 * Debug	None
**/

#ifndef __KinetisKE_CSL_ACMP_H
#define __KinetisKE_CSL_ACMP_H

#include "KinetisKE_csl.h"

/**
 * ACMP Initializing Structure(except DAC)
**/
typedef struct
{
	uint8_t Hysterisis;						//Hysterisis of ACMP
	uint8_t Edge;							//Trigger Edge
	uint8_t Positive;						//Positive Input
	uint8_t Negative;						//Negative Input
}ACMP_InitTypeDef;

/**
 * DAC Configuration
**/
typedef struct
{
	FunctionalState State;					//DAC Enable or not
	uint8_t Reference;						//DAC Voltage Reference
	uint8_t Value;							//DAC Output Value
}DAC_ConfigTypeDef;

/**
 * ACMP Status Defination
**/
typedef enum
{
	CSL_ACMP_STATE_RESET = 0x00u,			//Analog Comparator is RESET
	CSL_ACMP_STATE_READY = 0x01u,			//Analog Comparator is READY
	CSL_ACMP_STATE_EFFECTIVE = 0x02u,		//Analog Comparator detected effective edge
}ACMP_StateTypeDef;

/**
 * ACMP Handle
**/
typedef struct
{
	ACMP_Type* 			Instance;			//Instance of ACMP
	ACMP_InitTypeDef 	Init;				//ACMP Init
	DAC_ConfigTypeDef 	DAConfig;			//DAC Configuration
	ACMP_StateTypeDef 	gState;				//Global State
}ACMP_HandleTypeDef;

/**
 * ACMP Hysterisis
**/
#define ACMP_HYST_20mV					0x00u
#define ACMP_HYST_30mV					ACMP_CS_HYST_MASK

/**
 * ACMP effective edges
**/
#define ACMP_EDGE_RISING				0x01u
#define ACMP_EDGE_FALLING				0x02u
#define ACMP_EDGE_BOTH_EDGES			ACMP_CS_ACMOD_MASK

/**
 * ACMP Input Selection
 * 		@arg	ACMP0	IN0->PTA0	IN1->PTA1	IN2->PTC4
 * 		@arg	ACMP1	IN0->PTA6	IN1->PTA7	IN2->PTB4
**/
#define ACMP_INPUT_EXT0					0x00u					//External Input0
#define ACMP_INPUT_EXT1					0x01u					//External Input1
#define ACMP_INPUT_EXT2					0x02u					//External Input2
#define ACMP_INPUT_DAC					0x03u					//DAC 

/**
 * DAC Reference
**/
#define DAC_REFERENCE_BG				0x00u					//Bandgap reference
#define DAC_REFERENCE_VDDA				ACMP_C1_DACREF_MASK		//VDDA

/* Macros Functions of ACMP */
/**
 * @brief	Enable/Disable ACMP
**/
#define __CSL_ACMP_ENABLE(__HANDLE__)					(__HANDLE__->Instance->CS |= ACMP_CS_ACE_MASK)
#define __CSL_ACMP_DISABLE(__HANDLE__)					(__HANDLE__->Instance->CS &= ~ACMP_CS_ACE_MASK)

/**
 * @brief	Enable/Disable ACMP OUT on pins
 * 			@arg		ACMP0_OUT->PTA4
 *			@arg		ACMP1_OUT->PTB5
**/
#define __CSL_ACMP_OUTPUT_ENABLE(__HANDLE__)			(__HANDLE__->Instance->CS |= ACMP_CS_ACOPE_MASK)
#define __CSL_ACMP_OUTPUT_DISABEL(__HANDLE__)			(__HANDLE__->Instance->CS &= ~ACMP_CS_ACOPE_MASK)

/**
 * @brief 	Get State of ACMP
**/
#define __CSL_ACMP_GET_STATE(__HANDLE__, __FLAG__)		((__HANDLE__->Instance->CS & (__FLAG__)) ? SET : RESET)

/**
 * @brief	Enable/Disable DAC
**/
#define __CSL_ACMP_DAC_ENABLE(__HANDLE__)				(__HANDLE__->Instance->C1 |= ACMP_C1_DACEN_MASK)
#define __CSL_ACMP_DAC_DISABLE(__HANDLE__)				(__HANDLE__->Instance->C1 &= ~ACMP_C1_DACEN_MASK)

/**
 * @brief	Clear ACMP Interrupt Flag
**/
#define __CSL_ACMP_IT_FLAG_CLEAR(__HANDLE__)			(__HANDLE__->Instance->CS &= ~ACMP_CS_ACF_MASK)

/**
 * @brief	Enable/Disable ACMP Interrupt
**/
#define __CSL_ACMP_IT_ENABLE(__HANDLE__)				(__HANDLE__->Instance->CS |= ACMP_CS_ACIE_MASK)
#define __CSL_ACMP_IT_DISABLE(__HANDLE__)				(__HANDLE__->Instance->CS &= ~ACMP_CS_ACIE_MASK)

/**
 * @brief	Get ACMP Output
**/
#define __CSL_ACMP_GET_OUTPUT(__HANDLE__)				((__HANDLE__->Instance->CS & ACMP_CS_ACO_MASK) ? SET : RESET)

/* Functions of ACMP */
CSL_StatusTypeDef CSL_ACMP_Init(ACMP_HandleTypeDef* cacmp);
CSL_StatusTypeDef CSL_ACMP_DeInit(ACMP_HandleTypeDef* cacmp);
void CSL_ACMP_MspInit(ACMP_HandleTypeDef* cacmp);
void CSL_ACMP_MspDeInit(ACMP_HandleTypeDef* cacmp);

FunctionalState CSL_ACMP_GetComparing(ACMP_HandleTypeDef* cacmp);
CSL_StatusTypeDef CSL_ACMP_SetDACValue(ACMP_HandleTypeDef* cacmp, uint8_t Value);
uint8_t CSL_ACMP_GetDACValue(ACMP_HandleTypeDef* cacmp);
ACMP_StateTypeDef CSL_ACMP_GetState(ACMP_HandleTypeDef* cacmp);

void CSL_ACMP_IRQHandler(ACMP_HandleTypeDef* cacmp);
void CSL_ACMP_Callback(ACMP_HandleTypeDef* cacmp);

/* Defgroup ACMP_Private_Macros Private Macros */
#define IS_ACMP_HYST(hyst)				((hyst == ACMP_HYST_20mV) || (hyst == ACMP_HYST_30mV))
#define IS_ACMP_EDGE(edge)				((edge == 0x00u) || \
										 (edge == ACMP_EDGE_RISING) || \
										 (edge == ACMP_EDGE_FALLING) || \
										 (edge == ACMP_EDGE_BOTH_EDGES))
#define IS_ACMP_INPUT(in)				((in == ACMP_INPUT_EXT0) || \
										 (in == ACMP_INPUT_EXT1) || \
										 (in == ACMP_INPUT_EXT2) || \
										 (in == ACMP_INPUT_DAC))
#define IS_DAC_REF(ref)					((ref == DAC_REFERENCE_BG) || (ref == DAC_REFERENCE_VDDA))

#endif /*__KinetisKE_CSL_ACMP_H*/

//EOF

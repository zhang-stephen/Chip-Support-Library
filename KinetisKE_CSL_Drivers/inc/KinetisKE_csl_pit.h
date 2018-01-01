/**
 * Title 	PIT module in CSL for KEAZ128(Header File)
 * License	GPLv2.0
 * Author	Stark Zhang
 * Debug	None
**/

/* KEA128 & KEA64 & KEA8 contain two channels in PIT module */

#ifndef __KinetisKE_CSL_PIT_H
#define __KinetisKE_CSL_PIT_H

#include "KinetisKE_csl.h"

/**
 * PIT Status TypeDef
**/
typedef enum
{
	CSL_PIT_STATE_RESET		= 0x00u,	//PIT is RESET(Disabled)
	CSL_PIT_STATE_READY		= 0x01u,	//PIT is Ready to run
	CSL_PIT_STATE_RUNNING	= 0x02u,	//PIT is Running
	CSL_PIT_STATE_STOP		= 0x03u,	//PIT is stopped by some operation(can be set to run again)
	CSL_PIT_STATE_TIMEUP	= 0x04u,	//PIT is time up
}PIT_StateTypeDef;

/**
 * PIT Initializing Structure
**/
typedef struct
{
	uint32_t Period;					//Counter Period of PIT
	FunctionalState State;				//Channel Enable or not
	FunctionalState Cascade;			//Chain Mode enabled or not(don't use it in Channel 0)
	PIT_StateTypeDef CState;			//Status of Channel(All of Status is accessing)
}PIT_InitTypeDef;

/**
 * PIT Handle Structure
**/
typedef struct
{
	PIT_StateTypeDef gState;			//Global State(RESET/READY/RUNNING)
	PIT_InitTypeDef  Init[2];			//Init of PIT Channel 0 and 1
}PIT_HandleTypeDef;

/* Macros of PIT */
/**
 * Channels of PIT
**/
#define PIT_CHANNEL_0					0x00u
#define PIT_CHANNEL_1					0x01u
#define PIT_CHANNEL_ALL					0xFFu

/* Macro Functions of PIT */
/**
 * @brief	Enable/Disable PIT module
**/
#define __CSL_PIT_ENABLE()				(PIT->MCR &= ~PIT_MCR_MDIS_MASK)
#define __CSL_PIT_DISABLE()				(PIT->MCR |= PIT_MCR_MDIS_MASK)

/**
 * @brief	Enable/Disable PIT Interrupt on each Channel
 * @param	ch
				Channels in PIT
				@arg 	PIT_CHANNEL_0
				@arg	PIT_CHANNEL_1
**/
#define __CSL_PIT_IT_ENABLE(ch)			(PIT->CHANNEL[ch].TCTRL |= PIT_TCTRL_TIE_MASK)
#define __CSL_PIT_IT_DISABLE(ch)		(PIT->CHANNEL[ch].TCTRL &= ~PIT_TCTRL_TIE_MASK)

/**
 * @brief	Clear PIT Interrupt Flag
 * @param	ch
				Channels in PIT
				@arg 	PIT_CHANNEL_0
				@arg	PIT_CHANNEL_1
**/
#define __CSL_PIT_FLAG_CLEAR(ch)		(PIT->CHANNEL[ch].TFLG |= PIT_TFLG_TIF_MASK)

/**
 * @brief	get PIT Interrupt Flag
 * @param	ch
				Channels in PIT
				@arg 	PIT_CHANNEL_0
				@arg	PIT_CHANNEL_1
**/
#define __CSL_PIT_GET_IT_FLAG(ch)		((PIT->CHANNEL[ch].TFLG & PIT_TFLG_TIF_MASK) ? SET : RESET)

/* Functions of PIT Module */
CSL_StatusTypeDef CSL_PIT_Init(PIT_HandleTypeDef* cpit);
CSL_StatusTypeDef CSL_PIT_DeInit(PIT_HandleTypeDef* cpit);
void CSL_PIT_MspInit(PIT_HandleTypeDef* cpit);
void CSL_PIT_MspDeInit(PIT_HandleTypeDef* cpit);

CSL_StatusTypeDef CSL_PIT_Start(PIT_HandleTypeDef* cpit, uint8_t Channel);
CSL_StatusTypeDef CSL_PIT_Stop(PIT_HandleTypeDef* cpit, uint8_t Channel);

void CSL_PIT_IRQHandler(PIT_HandleTypeDef* cpit, uint8_t Channel);
void CSL_PIT_PeriodElapsedCallback(PIT_HandleTypeDef* cpit, uint8_t Channel);

/* There is no necessary to check parameter */

#endif /*__KinetisKE_CSL_PIT_H*/

//EOF

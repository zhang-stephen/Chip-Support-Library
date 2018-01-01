/**
 * Title 	PIT module in CSL for KEAZ128(Source File)
 * License	GPLv2.0
 * Author	Stark Zhang
 * Debug	None
**/

#include "KinetisKE_csl_pit.h"

/* Public Definations */
/**
 * @brief
 * @param
 * @return
 * @note
**/
CSL_StatusTypeDef CSL_PIT_Init(PIT_HandleTypeDef* cpit)
{
	//Parameter Check
	if(cpit == NULL)
	{
		return CSL_Error;
	}
	
	//Enable the PIT Module
	__CSL_PIT_ENABLE();
	
	//Channel 0 is enabled
	if(cpit->Init[0].State == SET)
	{
		//Set Period
		PIT->CHANNEL[0].LDVAL = cpit->Init[0].Period;
		
		//Set Channel State
		cpit->Init[0].CState = CSL_PIT_STATE_READY;
	}
	
	//Channel 1 is enabled
	if(cpit->Init[1].State == SET)
	{
		//Set Period
		PIT->CHANNEL[1].LDVAL = cpit->Init[1].Period;
		
		//Set Channel State
		cpit->Init[1].CState = CSL_PIT_STATE_READY;
	}
	
	//Channel 1 Chain Mode is SET
	if(cpit->Init[1].Cascade == SET)
	{
		//Channel 0 is not enabled
		if(cpit->Init[0].State != SET)
		{
			return CSL_Error;
		}
		
		PIT->CHANNEL[1].TCTRL |= PIT_TCTRL_CHN_MASK;
	}
	
	//Msp Init
	CSL_PIT_MspInit(cpit);
	
	//Set PIT global State
	cpit->gState = CSL_PIT_STATE_RUNNING;
	
	return CSL_OK;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
CSL_StatusTypeDef CSL_PIT_DeInit(PIT_HandleTypeDef* cpit)
{
	//Parameter Check
	if(cpit == NULL)
	{
		return CSL_Error;
	}
	
	//Reset Rigisters
	if(cpit->Init[0].State == SET)
	{
		cpit->Init[0].State = RESET;
		PIT->CHANNEL[0].TCTRL = (uint32_t)0x00u;
		PIT->CHANNEL[0].LDVAL = (uint32_t)0x00u;
		__CSL_PIT_FLAG_CLEAR(PIT_CHANNEL_0);
		cpit->Init[0].CState = CSL_PIT_STATE_RESET;
	}
	
	if(cpit->Init[1].State == SET)
	{
		cpit->Init[1].State = RESET;
		cpit->Init[1].Cascade = RESET;
		PIT->CHANNEL[1].TCTRL = (uint32_t)0x00u;
		PIT->CHANNEL[1].LDVAL = (uint32_t)0x00u;
		__CSL_PIT_FLAG_CLEAR(PIT_CHANNEL_1);
		cpit->Init[1].CState = CSL_PIT_STATE_RESET;
	}
	
	//Set global State
	cpit->gState = CSL_PIT_STATE_RESET;
	
	//Msp DeInit
	CSL_PIT_MspDeInit(cpit);
	
	//Disable PIT
	PIT->MCR = (uint32_t)0x06u;
	
	return CSL_OK;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
__weak void CSL_PIT_MspInit(PIT_HandleTypeDef* cpit)
{
	UNUSED(cpit);
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
__weak void CSL_PIT_MspDeInit(PIT_HandleTypeDef* cpit)
{
	UNUSED(cpit);
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
CSL_StatusTypeDef CSL_PIT_Start(PIT_HandleTypeDef* cpit, uint8_t Channel)
{
	//Parameter Check
	if(cpit == NULL)
	{
		return CSL_Error;
	}
	
	//Enable the PIT Channels
	switch(Channel)
	{
	case PIT_CHANNEL_0:
		PIT->CHANNEL[PIT_CHANNEL_0].TCTRL |= PIT_TCTRL_TEN_MASK;
		cpit->Init[PIT_CHANNEL_0].CState = CSL_PIT_STATE_RUNNING;
		break;
	case PIT_CHANNEL_1:
		PIT->CHANNEL[PIT_CHANNEL_1].TCTRL |= PIT_TCTRL_TEN_MASK;
		cpit->Init[PIT_CHANNEL_1].CState = CSL_PIT_STATE_RUNNING;
		break;
	case PIT_CHANNEL_ALL:
		PIT->CHANNEL[PIT_CHANNEL_0].TCTRL |= PIT_TCTRL_TEN_MASK;
		cpit->Init[PIT_CHANNEL_0].CState = CSL_PIT_STATE_RUNNING;
		PIT->CHANNEL[PIT_CHANNEL_1].TCTRL |= PIT_TCTRL_TEN_MASK;
		cpit->Init[PIT_CHANNEL_1].CState = CSL_PIT_STATE_RUNNING;
		break;
	default:
		return CSL_Error;
		break;
	}
	
	return CSL_OK;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
CSL_StatusTypeDef CSL_PIT_Stop(PIT_HandleTypeDef* cpit, uint8_t Channel)
{
	//Parameter Check
	if(cpit == NULL)
	{
		return CSL_Error;
	}
	
	//Enable the PIT Channels
	switch(Channel)
	{
	case PIT_CHANNEL_0:
		PIT->CHANNEL[PIT_CHANNEL_0].TCTRL &= ~PIT_TCTRL_TEN_MASK;
		cpit->Init[PIT_CHANNEL_0].CState = CSL_PIT_STATE_STOP;
		break;
	case PIT_CHANNEL_1:
		PIT->CHANNEL[PIT_CHANNEL_1].TCTRL &= ~PIT_TCTRL_TEN_MASK;
		cpit->Init[PIT_CHANNEL_1].CState = CSL_PIT_STATE_STOP;
		break;
	case PIT_CHANNEL_ALL:
		PIT->CHANNEL[PIT_CHANNEL_0].TCTRL &= ~PIT_TCTRL_TEN_MASK;
		cpit->Init[PIT_CHANNEL_0].CState = CSL_PIT_STATE_STOP;
		PIT->CHANNEL[PIT_CHANNEL_1].TCTRL &= ~PIT_TCTRL_TEN_MASK;
		cpit->Init[PIT_CHANNEL_1].CState = CSL_PIT_STATE_STOP;
		break;
	default:
		return CSL_Error;
		break;
	}
	
	return CSL_OK;
}

void CSL_PIT_IRQHandler(PIT_HandleTypeDef* cpit, uint8_t Channel)
{
	//Flag is SET
	if(__CSL_PIT_GET_IT_FLAG(Channel) == SET)
	{
		//Clear Flag
		__CSL_PIT_FLAG_CLEAR(Channel);
		
		//PIT Channel is Time-up
		cpit->Init[Channel].CState = CSL_PIT_STATE_TIMEUP;
		
		//User Callback
		CSL_PIT_PeriodElapsedCallback(cpit, Channel);
		
		//PIT Channel is running
		cpit->Init[Channel].CState = CSL_PIT_STATE_RUNNING;
	}
}

__weak void CSL_PIT_PeriodElapsedCallback(PIT_HandleTypeDef* cpit, uint8_t Channel)
{
	UNUSED(cpit);
	UNUSED(Channel);
}


//EOF

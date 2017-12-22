/**
 * Title 	Watch Dog module in CSL for KEAZ128(Source File)
 * License	GPLv2.0
 * Author	Stark Zhang
 * Debug	None
**/

#include "KinetisKE_csl_wdog.h"

/**
 * @brief
 * @param
 * @return
 * @note
**/
CSL_StatusTypeDef CSL_WDOG_Config(WDOG_HandleTypeDef* cwdog)
{
	//Parameters Check
	if(cwdog == NULL)
	{
		return CSL_Error;
	}
	
	//Unlock WDOG
	__CSL_WDOG_UNLOCK();
	
	//Write Watch Dog Registers
	WDOG->TOVAL = cwdog->CountValue;
	
	//Window Mode 
	if(cwdog->Mode == WDOG_MODE_WINDOW)
	{
		WDOG->WIN = cwdog->WindowValue;
	}
	
	WDOG->CS2 = cwdog->Mode | cwdog->PreScaler | cwdog->ClockSource;
	
	//Msp Configure
	CSL_WDOG_MspConfig(cwdog);
	
	//Enable Watch Dog
	WDOG->CS1 = WDOG_CS1_EN_MASK | WDOG_CS1_DBG_MASK | WDOG_CS1_WAIT_MASK | WDOG_CS1_STOP_MASK;
	
	return CSL_OK;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
__weak void CSL_WDOG_MspConfig(WDOG_HandleTypeDef* cwdog)
{
    UNUSED(cwdog);
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
void CSL_WDOG_Refresh(WDOG_HandleTypeDef* cwdog)
{
	//Disable the Global Interrupts
	__CSL_GIRQ_DISABLE();
	
	//Feed The WDOG
	__CSL_WODG_REFRESH();
	
	//Ensable the Global Interrupts
	__CSL_GIRQ_ENABLE();
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
void CSL_WDOG_IRQHandler(WDOG_HandleTypeDef* cwdog)
{
    //Clear IT Flag
    __CSL_WDOG_CLEAR_FLAG();

    //User Operation(callback)
    CSL_WDOG_Callback(cwdog);
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
__weak void CSL_WDOG_Callback(WDOG_HandleTypeDef* cwdog)
{
    UNUSED(cwdog);
}

//EOF

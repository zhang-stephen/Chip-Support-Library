/**
 * Title 	FlexTimer module in CSL for KEAZxxx(Source File)
 * License	GPLv2.0
 * Author	Stark Zhang
 * Debug	None
**/

#include "KinetisKE_csl_ftm.h"

/**
 * @brief
 * @param
 * @return
 * @note
**/
CSL_StatusTypeDef CSL_FTM_Base_Init(FTM_HandleTypeDef* cftm)
{
	//Parameter Check
	if(cftm == NULL)
	{
		return CSL_Error;
	}
	
	//Disable Write Protection
	if(cftm->Instance == FTM2)
	{
		__CSL_FTM_WP_DISABLE(cftm->Instance);
	}
	
	//Basic Configure FTM
	cftm->Instance->MOD = cftm->Init.Period;
	cftm->Instance->SC = cftm->Init.CountMode;
	
	//Enable Write Protection
	if(cftm->Instance == FTM2)
	{
		__CSL_FTM_WP_ENABLE(cftm->Instance);
	}
	
	return CSL_OK;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
CSL_StatusTypeDef CSL_FTM_Base_DeInit(FTM_HandleTypeDef* cftm)
{}

/**
 * @brief
 * @param
 * @return
 * @note
**/
__weak void CSL_FTM_Base_MspInit(FTM_HandleTypeDef* cftm)
{
	UNUSED(cftm);
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
__weak void CSL_FTM_Base_MspDeInit(FTM_HandleTypeDef* cftm)
{
	UNUSED(cftm);
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
CSL_StatusTypeDef CSL_FTM_Base_Start(FTM_HandleTypeDef* cftm)
{
	if(cftm == NULL)
	{
		return CSL_Error;
	}
	
	assert_param(IS_FTM_CLKSource(cftm->Init.ClockSource));
	assert_param(IS_FTM_PreScaler(cftm->Init.PreScaler));
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
CSL_StatusTypeDef CSL_FTM_Base_Stop(FTM_HandleTypeDef* cftm)
{}

/**
 * @brief
 * @param
 * @return
 * @note
**/
CSL_StatusTypeDef CSL_FTM_Base_Start_IT(FTM_HandleTypeDef* cftm)
{}

/**
 * @brief
 * @param
 * @return
 * @note
**/
CSL_StatusTypeDef CSL_FTM_Base_Stop_IT(FTM_HandleTypeDef* cftm)
{}
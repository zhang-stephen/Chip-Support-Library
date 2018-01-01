/**
 * Title 	Analog Comparator(ACMP) module in CSL for KEAZ128(Header File)
 * License	GPLv2.0
 * Author	Stark Zhang
 * Debug	None
**/

#include "KinetisKE_csl_acmp.h"

/* Private Functions Declarations */
static CSL_StatusTypeDef DAC_Config(ACMP_HandleTypeDef* cacmp);

/* Public Fucntions Definations */
/**
 * @brief
 * @param
 * @return
 * @note
**/
CSL_StatusTypeDef CSL_ACMP_Init(ACMP_HandleTypeDef* cacmp)
{
	//Parameters Check
	if((cacmp == NULL) || (cacmp->Init.Positive == cacmp->Init.Negative))
	{
		return CSL_Error;
	}
	
	assert_param(IS_ACMP_HYST(cacmp->Init.Hysterisis));
	assert_param(IS_ACMP_EDGE(cacmp->Init.Edge));
	assert_param(IS_ACMP_INPUT(cacmp->Init.Positive));
	assert_param(IS_ACMP_INPUT(cacmp->Init.Negative));
	
	//at least one input here is a DAC
	if((cacmp->Init.Positive == ACMP_INPUT_DAC) || (cacmp->Init.Negative == ACMP_INPUT_DAC))
	{	
		//Configure the DAC
		if(DAC_Config(cacmp) != CSL_OK)
		{
			return CSL_Error;
		}
	}
	
	//Configure ACMP
	cacmp->Instance->CS = cacmp->Init.Hysterisis | cacmp->Init.Edge;
	cacmp->Instance->C0 = cacmp->Init.Positive | cacmp->Init.Negative;
	cacmp->Instance->C2 = 1 << cacmp->Init.Positive | 1 << cacmp->Init.Negative;
	
	//User Configure
	CSL_ACMP_MspInit(cacmp);
	
	//Enable ACMP
	__CSL_ACMP_ENABLE(cacmp);
	
	//ACMP is Ready
	cacmp->gState = CSL_ACMP_STATE_READY;
	
	return CSL_OK;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
CSL_StatusTypeDef CSL_ACMP_DeInit(ACMP_HandleTypeDef* cacmp)
{
	//Parameters Check
	if(cacmp == NULL)
	{
		return CSL_Error;
	}
	
	//Reset ACMP Registers
	cacmp->Instance->CS = 0x00u;
	cacmp->Instance->C0 = 0x00u;
	cacmp->Instance->C1 = 0x00u;
	cacmp->Instance->C2 = 0x00u;
	
	//User RESET
	CSL_ACMP_MspDeInit(cacmp);
	
	//ACMP is RESET
	cacmp->gState = CSL_ACMP_STATE_RESET;
	
	return CSL_OK;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
__weak void CSL_ACMP_MspInit(ACMP_HandleTypeDef* cacmp)
{
	UNUSED(cacmp);
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
__weak void CSL_ACMP_MspDeInit(ACMP_HandleTypeDef* cacmp)
{
	UNUSED(cacmp);
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
FunctionalState CSL_ACMP_GetComparing(ACMP_HandleTypeDef* cacmp)
{
	//ACMP Output is effective
	if(__CSL_ACMP_GET_STATE(cacmp, ACMP_CS_ACO_MASK) == SET)
	{
		//Clear ACMP Interrupt Flag
		__CSL_ACMP_IT_FLAG_CLEAR(cacmp);
		
		return SET;
	}
	else return RESET;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
CSL_StatusTypeDef CSL_ACMP_SetDACValue(ACMP_HandleTypeDef* cacmp, uint8_t Value)
{
	//Parameters Check
	if((cacmp->DAConfig.State != SET) || ((cacmp->Instance->C1 & ACMP_C1_DACEN_MASK) == 0x00u))
	{
		return CSL_Error;
	}
	
	//Disable DAC
	__CSL_ACMP_DAC_DISABLE(cacmp);
	
	//Modify DAC Register
	cacmp->Instance->C1 &= ~ACMP_C1_DACVAL_MASK;
	cacmp->Instance->C1 |= Value;
	
	//Modify DAC configuration in Structure
	cacmp->DAConfig.Value = Value;
	
	//Enable DAC
	__CSL_ACMP_DAC_ENABLE(cacmp);
	
	return CSL_OK;
	
}

/**
 * @brief
 * @param
 * @return	uint8_t
				@arg	0xFFu means DAC is Disabled
 * @note 
**/
uint8_t CSL_ACMP_GetDACValue(ACMP_HandleTypeDef* cacmp)
{
	//DAC is not enabled
	if((cacmp->DAConfig.State != SET) || ((cacmp->Instance->C1 & ACMP_C1_DACEN_MASK) == 0x00u))
	{
		return 0xFFu;
	}
	else
		return (cacmp->Instance->C1 & ACMP_C1_DACVAL_MASK);
}

/**
 * @brief
 * @param
 * @return	
 * @note 
**/
ACMP_StateTypeDef CSL_ACMP_GetState(ACMP_HandleTypeDef* cacmp)
{
	return cacmp->gState;
}

/**
 * @brief
 * @param
 * @return	
 * @note 
**/
void CSL_ACMP_IRQHandler(ACMP_HandleTypeDef* cacmp)
{
	//Clear Interrupt Flag
	__CSL_ACMP_IT_FLAG_CLEAR(cacmp);
	
	//ACMP State is Effective
	cacmp->gState = CSL_ACMP_STATE_EFFECTIVE;
	
	//User Callback Function
	 CSL_ACMP_Callback(cacmp);
	 
	//ACMP State is Ready
	cacmp->gState = CSL_ACMP_STATE_READY;
}

/**
 * @brief
 * @param
 * @return	
 * @note 
**/
__weak void CSL_ACMP_Callback(ACMP_HandleTypeDef* cacmp)
{
	UNUSED(cacmp);
}

/* Private Functions Declarations */
static CSL_StatusTypeDef DAC_Config(ACMP_HandleTypeDef* cacmp)
{
	//Parameters Check
	if((cacmp == NULL) || (cacmp->DAConfig.State != SET))
	{
		return CSL_Error;
	}
	
	assert_param(IS_DAC_REF(cacmp->DAConfig.Reference));
	
	//configure DAC
	cacmp->Instance->C1 = cacmp->DAConfig.Reference | (cacmp->DAConfig.Value & 0x3Fu);
	
	//Enable DAC
	__CSL_ACMP_DAC_ENABLE(cacmp);
	
	return CSL_OK;
}

//EOF

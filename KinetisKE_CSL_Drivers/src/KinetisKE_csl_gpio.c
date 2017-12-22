/**
 * Title 	GPIO/FGPIO module in CSL for KEAZ128(Source File)
 * License	GPLv2.0
 * Author	Stark Zhang
 * Debug	None
**/

#include "KinetisKE_csl_gpio.h"

/**
 * @brief	GPIO Initialized
 * @param	GPIO_Type* GPIOx
				GPIOA/GPIOB/GPIOC
 * @param	GPIO_InitTypeDef* GPIO_Init
				structure of GPIO_Init
 * @return	None
 * @note	None
**/
void CSL_GPIO_Init(GPIO_Type* GPIOx, GPIO_InitTypeDef* GPIO_Init)
{
	assert_param(IS_GPIO_GPIOx(GPIOx));
	
	/* write GPIO Registers */
	//Configure as Output
	if(GPIO_Init->Mode == GPIO_Mode_Output)
	{
		GPIOx->PDDR = GPIO_Init->Pin;
	}
	//Configure as Input
	else if(GPIO_Init->Mode == GPIO_Mode_Input)
	{
		//Enable Input
		GPIOx->PIDR = GPIO_PIDR_PID_MASK ^ GPIO_Init->Pin;
		GPIOx->PDDR = GPIO_PDDR_PDD_MASK ^ GPIO_Init->Pin; 
	}
	
	/* write PORT Registers */
	if(GPIOx == GPIOA)
	{
		//Pull up
		if(GPIO_Init->Pull == GPIO_PUPD_UP)
		{
			PORT->PUE0 = GPIO_Init->Pin;
		}
		//No pull 
		else 
		{
			PORT->PUE0 = 0xFFFFFFFFU ^ GPIO_Init->Pin;
		}
	}
	else if(GPIOx == GPIOB)
	{
		//Pull up
		if(GPIO_Init->Pull == GPIO_PUPD_UP)
		{
			PORT->PUE1 = GPIO_Init->Pin;
		}
		//No pull 
		else 
		{
			PORT->PUE1 = 0xFFFFFFFFU ^ GPIO_Init->Pin;
		}
	}
	else if(GPIOx == GPIOC)
	{
		//Pull up
		if(GPIO_Init->Pull == GPIO_PUPD_UP)
		{
			PORT->PUE2 = GPIO_Init->Pin;
		}
		//No pull 
		else 
		{
			PORT->PUE2 = 0xFFFFFFFFU ^ GPIO_Init->Pin;
		}
	}
	//Error Occured
	else __no_operation();

	//User Configuration
	CSL_PORT_MspInit();
}

/**
 * @brief	GPIO Deinitialized
 * @param	GPIO_Type* GPIOx
				GPIOA/GPIOB/GPIOC
 * @param	uint32_t Pin
				pins
 * @return	None
 * @note	None
**/
void CSL_GPIO_DeInit(GPIO_Type* GPIOx, uint32_t Pin)
{
	assert_param(IS_GPIO_GPIOx(GPIOx));
	CLEAR_BIT(GPIOx->PDOR, Pin);		//clear output
	CLEAR_BIT(GPIOx->PDDR, Pin);		//reset pins to input
	SET_BIT(GPIOx->PIDR, Pin);			//disable input

	//User Configuration
	CSL_PORT_MspDeInit();
}

/**
 * @brief	Write GPIO logic Level
 * @param	GPIO_MemMapPtr GPIOx
				GPIO Instance
 * @param	uint32_t Pin
				Pins
 * @param	GPIO_LogicTypeDef logic
				logic level
 * @return 	None
 * @note	None
**/
void CSL_GPIO_WritePin(GPIO_MemMapPtr GPIOx, uint32_t Pin, GPIO_LogicTypeDef logic)
{
	assert_param(IS_GPIO_GPIOx(GPIOx));
	assert_param(IS_GPIO_Logic(logic));
	
	//Logic is Low Level
	if(logic == GPIO_Logic_HiZ || logic == GPIO_Logic_0)
	{
		GPIOx->PCOR = Pin;
	}
	//Logic is High Level
	else if(logic == GPIO_Logic_1)
	{
		GPIOx->PSOR = Pin;
	}
	else __no_operation();
}

/**
 * @brief	Toggle GPIO logic Level
 * @param	GPIO_MemMapPtr GPIOx
				GPIO Instance
 * @param	uint32_t Pin
				Pins
 * @return 	None
 * @note	None
**/
void CSL_GPIO_TogglePin(GPIO_MemMapPtr GPIOx, uint32_t Pin)
{
	assert_param(IS_GPIO_GPIOx(GPIOx));
	
	GPIOx->PTOR = Pin;
}

/**
 * @brief	Read Input data on a specific Pin
 * @param	GPIO_MemMapPtr GPIOx
				GPIO Instance
 * @param	uint32_t Pin
				Pins
 * @return 	GPIO_LogicTypeDef
				Logic Level
 * @note	Only for one specific Pin!
**/
GPIO_LogicTypeDef CSL_GPIO_ReadPin(GPIO_MemMapPtr GPIOx, uint32_t Pin)
{
	assert_param(IS_GPIO_GPIOx(GPIOx));
	
	GPIO_LogicTypeDef logic = GPIO_Logic_HiZ;
	
	//Pin is Output, return Hiz
	if((GPIOx->PDDR & Pin) != 0)
	{
		logic = GPIO_Logic_HiZ;
	}
	//Pin is input
	else if((GPIOx->PDDR & Pin) == 0)
	{
		//Input Disabled
		if((GPIOx->PIDR & Pin) != 0)
		{
			logic = GPIO_Logic_HiZ;
		}
		//Input Enabled
		else
		{
			if((GPIOx->PDIR & Pin) == 0)
				logic = GPIO_Logic_0;
			else logic = GPIO_Logic_1;
		}
	}
	
	return logic;
}

/**
 * @brief	Enable High Current Driver(optional)
 * @param	uint32_t GPIO_HDRVE_Pin
				Pins that those are with HDRVE
 * @return 	None
 * @note	Only some pins can enable it & active to FGPIO
**/
void CSL_GPIO_EnableHDRVE(uint32_t GPIO_HDRVE_Pin)
{
	assert_param(IS_GPIO_HDRVE(GPIO_HDRVE_Pin));
	
	PORT->HDRVE = GPIO_HDRVE_Pin;
}

/**
 * @brief	Disable High Current Driver(optional)
 * @param	uint32_t GPIO_HDRVE_Pin
				Pins that those are with HDRVE
 * @return 	None
 * @note	Only some pins can enable it & active to FGPIO
**/
void CSL_GPIO_DisableHDRVE(uint32_t GPIO_HDRVE_Pin)
{
	assert_param(IS_GPIO_HDRVE(GPIO_HDRVE_Pin));
	
	PORT->HDRVE = 0xFFU ^ GPIO_HDRVE_Pin;
}

/**
 * @brief	Port Configuration by Users
 * @param	None
 * @return 	None
 * @note	None
**/
__weak void CSL_PORT_MspInit(void)
{
	__no_operation();
}

/**
 * @brief	Port Cancel Configuration by Users
 * @param	None
 * @return 	None
 * @note	None
**/
__weak void CSL_PORT_MspDeInit(void)
{
	__no_operation();
}

//EOF

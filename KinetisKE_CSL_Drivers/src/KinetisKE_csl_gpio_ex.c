/**
 * Title 	GPIO/Port module in CSL for KEAZ128(Source File)
 * License	GPLv2.0
 * Author	Stark Zhang
 * Debug	None
**/

#include "KinetisKE_csl_gpio_ex.h"

/**
 * @brief	FGPIO Initialized
 * @param	FGPIO_InitTypeDef* FGPIO_Init
				structure of FGPIO_Init
 * @return	None
 * @note	None
**/
void CSL_FGPIO_Init(FGPIO_Type* FGPIOx, FGPIO_InitTypeDef* FGPIO_Init)
{
	assert_param(IS_GPIO_FGPIOx(FGPIOx));
	
	/* write FGPIO Registers */
	//Configure as Output
	if(FGPIO_Init->Mode == GPIO_Mode_Output)
	{
		FGPIOx->PDDR = FGPIO_Init->Pin;
	}
	//Configure as Input
	else if(FGPIO_Init->Mode == GPIO_Mode_Input)
	{
		//Enable Input
		FGPIOx->PIDR = FGPIO_PIDR_PID_MASK ^ FGPIO_Init->Pin;
		FGPIOx->PDDR = FGPIO_PDDR_PDD_MASK ^ FGPIO_Init->Pin; 
	}
	
	/* write PORT Registers */
	if(FGPIOx == FGPIOA)
	{
		//Pull up
		if(FGPIO_Init->Pull == GPIO_PUPD_UP)
		{
			PORT->PUE0 = FGPIO_Init->Pin;
		}
		//No pull 
		else 
		{
			PORT->PUE0 = 0xFFFFFFFFU ^ FGPIO_Init->Pin;
		}
	}
	else if(FGPIOx == FGPIOB)
	{
		//Pull up
		if(FGPIO_Init->Pull == GPIO_PUPD_UP)
		{
			PORT->PUE1 = FGPIO_Init->Pin;
		}
		//No pull 
		else 
		{
			PORT->PUE1 = 0xFFFFFFFFU ^ FGPIO_Init->Pin;
		}
	}
	else if(FGPIOx == FGPIOC)
	{
		//Pull up
		if(FGPIO_Init->Pull == GPIO_PUPD_UP)
		{
			PORT->PUE2 = FGPIO_Init->Pin;
		}
		//No pull 
		else 
		{
			PORT->PUE2 = 0xFFFFFFFFU ^ FGPIO_Init->Pin;
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
void CSL_FGPIO_DeInit(FGPIO_Type* FGPIOx, uint32_t Pin)
{
	assert_param(IS_GPIO_FGPIOx(FGPIOx));
	CLEAR_BIT(FGPIOx->PDOR, Pin);		//clear output
	CLEAR_BIT(FGPIOx->PDDR, Pin);		//reset pins to input
	SET_BIT(FGPIOx->PIDR, Pin);			//disable input

	//User Configuration
	CSL_PORT_MspDeInit();
}

/**
 * @brief	Write FGPIO logic Level
 * @param	FGPIO_Type* FGPIOx
				FGPIO Instance
 * @param	uint32_t Pin
				Pins
 * @param	GPIO_LogicTypeDef logic
				logic level
 * @return 	None
 * @note	None
**/
void CSL_FGPIO_WritePin(FGPIO_Type* FGPIOx, uint32_t Pin, GPIO_LogicTypeDef logic)
{
	assert_param(IS_GPIO_FGPIOx(FGPIOx));
	assert_param(IS_GPIO_Logic(logic));
	
	//Logic is Low Level
	if(logic == GPIO_Logic_HiZ || logic == GPIO_Logic_0)
	{
		FGPIOx->PCOR = Pin;
	}
	//Logic is High Level
	else if(logic == GPIO_Logic_1)
	{
		FGPIOx->PSOR = Pin;
	}
	else __no_operation();
}

/**
 * @brief	Toggle FGPIO logic Level
 * @param	FGPIO_MemMapPtr FGPIOx
				FGPIO Instance
 * @param	uint32_t Pin
				Pins
 * @return 	None
 * @note	None
**/
void CSL_FGPIO_TogglePin(FGPIO_MemMapPtr FGPIOx, uint32_t Pin)
{
	assert_param(IS_GPIO_FGPIOx(FGPIOx));
	
	FGPIOx->PTOR = Pin;
}

/**
 * @brief	Read Input data on a specific Pin
 * @param	FGPIO_MemMapPtr FGPIOx
				FGPIO Instance
 * @param	uint32_t Pin
				Pins
 * @return 	GPIO_LogicTypeDef
				Logic Level
 * @note	Only for one specific Pin!
**/
GPIO_LogicTypeDef CSL_FGPIO_ReadPin(FGPIO_MemMapPtr FGPIOx, uint32_t Pin)
{
	assert_param(IS_GPIO_FGPIOx(FGPIOx));
	
	GPIO_LogicTypeDef logic = GPIO_Logic_HiZ;
	
	//Pin is Output, return Hiz
	if((FGPIOx->PDDR & Pin) != 0)
	{
		logic = GPIO_Logic_HiZ;
	}
	//Pin is input
	else if((FGPIOx->PDDR & Pin) == 0)
	{
		//Input Disabled
		if((FGPIOx->PIDR & Pin) != 0)
		{
			logic = GPIO_Logic_HiZ;
		}
		//Input Enabled
		else
		{
			if((FGPIOx->PDIR & Pin) == 0)
				logic = GPIO_Logic_0;
			else logic = GPIO_Logic_1;
		}
	}
	
	return logic;
}

//EOF

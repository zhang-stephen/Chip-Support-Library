/**
 * Title 	ADC module in CSL for KEAZxxx(Source File)
 * License	GPLv2.0
 * Author	Stark Zhang
 * Debug	None
**/

#include "KinetisKE_csl_adc.h"

/**
 * Private Functions 
**/
static void ADC_Config(ADC_HandleTypeDef* cadc);
static void ADC_SetChannel(ADC_HandleTypeDef* cadc);


/* Public Functions */
/**
 * @brief	ADC Initialized
 * @param	ADC_HandleTypeDef* cadc
				ADC Handler
 * @return	CSL_StatusTypeDef
 * @note	None
**/
CSL_StatusTypeDef CSL_ADC_Init(ADC_HandleTypeDef* cadc)
{
	//Parameters Check
	if(cadc == NULL)
	{
		return CSL_Error;
	}
	
	assert_param(IS_ADC_Channel(cadc->Init.Channel));
	assert_param(IS_ADC_REF(cadc->Init.Reference));
	assert_param(IS_ADC_CLKSource(cadc->Init.Clock));
	assert_param(IS_ADC_CLKPSC(cadc->Init.PreScaler));
	
	//ADC Configure
	ADC_Config(cadc);
	
	//Msp Init
	CSL_ADC_MspInit(cadc);
	
	//ADC is Ready
	cadc->gState = CSL_ADC_STATE_READY;
	
	return CSL_OK;
}

/**
 * @brief	ADC Initialization Cancel
 * @param	ADC_HandleTypeDef* cadc
				ADC Handler
 * @return	CSL_StatusTypeDef
 * @note	None
**/
CSL_StatusTypeDef CSL_ADC_DeInit(ADC_HandleTypeDef* cadc)
{
	//Parameters Check
	if(cadc == NULL)
	{
		return CSL_Error;
	}
	
	//Reset the ADC Registers
	ADC->SC1 = 0x1Fu;			//Disable the ADC & Interrupt of ADC
	ADC->SC2 = 0x08u;
	ADC->SC3 = 0x00u;
	ADC->SC4 = 0x00u;
	ADC->SC5 = 0x00u;
	ADC->CV = 0x00u;
	ADC->APCTL1 = 0x0000u;
	
	//Msp Deinit
	CSL_ADC_MspDeInit(cadc);
	
	return CSL_OK;
}

/**
 * @brief	ADC Msp Initialized
 * @param	ADC_HandleTypeDef* cadc
				ADC Handler
 * @return	None
 * @note	None
**/
__weak void CSL_ADC_MspInit(ADC_HandleTypeDef* cadc)
{
	UNUSED(cadc);
}

/**
 * @brief	ADC Msp DeInitialized
 * @param	ADC_HandleTypeDef* cadc
				ADC Handler
 * @return	CSL_StatusTypeDef
 * @note	None
**/
__weak void CSL_ADC_MspDeInit(ADC_HandleTypeDef* cadc)
{
	UNUSED(cadc);
}

uint16_t CSL_ADC_GetSingleValue(ADC_HandleTypeDef* cadc)
{
	//Parameters Check
	if(cadc == NULL)
	{
		return 0xFFFFu;
	}
	
	//in single converting mode
	if(__CSL_ADC_GET_STATE(ADC->SC1, ADC_SC1_ADCO_MASK) == RESET)
	{
		//write the ADC->SC1 to start converting
		ADC_SetChannel(cadc);
		
		//wait the coco flag
		while(__CSL_ADC_GET_STATE(ADC->SC1, ADC_SC1_COCO_MASK) == SET)
			;
	}
	
	//Get Value
	return ADC->R;
}

/**
 * @brief	ADC Interrupt in CSL
 * @param	ADC_HandleTypeDef* cadc
				ADC Handler
 * @return	None
 * @note	None
**/
void CSL_ADC_IRQHandler(ADC_HandleTypeDef* cadc)
{
	if((__CSL_ADC_GET_STATE(ADC->SC1, ADC_SC1_COCO_MASK) && __CSL_ADC_GET_STATE(ADC->SC1, ADC_SC1_AIEN_MASK)) == SET)
	{
		//Disable the ADC Interrupt
		__CSL_ADC_IT_DISABLE();
		
		//ADC conversion is Complete
		cadc->gState = CSL_ADC_STATE_CPLT;
		
		//ADC conversion Complete Callback
		CSL_ADC_ConvCpltCallback(cadc);
		
		//ADC is Ready
		cadc->gState = CSL_ADC_STATE_READY;
		
		//Enable the ADC Interrupt
		__CSL_ADC_IT_ENABLE();
	}
}

/**
 * @brief	ADC Interrupt Callback
 * @param	ADC_HandleTypeDef* cadc
				ADC Handler
 * @return	None
 * @note	None
**/
__weak void CSL_ADC_ConvCpltCallback(ADC_HandleTypeDef* cadc)
{
	UNUSED(cadc);
}

/* Private Functions Definations */
/**
 * @brief 	ADC Configure
**/
static void ADC_Config(ADC_HandleTypeDef* cadc)
{	
	//Wirte ADC->SC3 Register
	ADC->SC3 = cadc->Init.PreScaler | cadc->Init.Length | cadc->Init.Clock;
	
	//Configure Trigger & Reference
	ADC->SC2 = cadc->Trigger.state | cadc->Init.Reference;
	
	//Compare Mode is Enabled
	if(cadc->Compare.state == SET)
	{
		//Write ADC->CV Register
		ADC->CV = cadc->Compare.CompValue;
		
		//Write ADC->CS2 Register
		SET_BIT(ADC->SC2, ADC_SC2_ACFE_MASK);
		if(cadc->Compare.CompMode == ADC_COMPARE_LOW)
		{
			CLEAR_BIT(ADC->SC2, ADC_SC2_ACFGT_MASK);
		}
		else
		{
			SET_BIT(ADC->SC2, ADC_SC2_ACFGT_MASK);
		}
	}
	
	//Hardware Trigger Source
	if(cadc->Trigger.state == ADC_TRIG_HARDWARE)
	{
		SIM->SOPT0 |= cadc->Trigger.HardTrigger;
	}
	
	//other Trigger Cofigure(allow Hard-Trig & Hardware trigger mask automatically when data fifo is not empty)
	ADC->SC5 = 0x1u;
	
	//FIFO Mode is Enabled
	if(cadc->FIFO.state == SET)
	{
		//Configure Trigger & Compare Mode
		ADC->SC4 = cadc->FIFO.TrigMode | cadc->FIFO.FifoComp | cadc->FIFO.Depth;
		
		//write Compare value
		ADC->CV = cadc->Compare.CompValue;
		
		//Scan Mode enabled
		if(cadc->FIFO.Scan == SET)
		{
			//Scan Mode enable
			SET_BIT(ADC->SC4, ADC_SC4_ASCANE_MASK);
			
			//Configure ADC Channel
			ADC->SC1 = cadc->FIFO.Channel[0];
			
			//Disable ADC pin Digital Logic
			ADC->APCTL1 |= 1 << cadc->FIFO.Channel[0];
		}
		//Scan mode is Disabled
		else
		{	
			for(uint8_t i = 0; i <= cadc->FIFO.Depth; i++)
			{
				
				ADC->APCTL1 |= 1 << cadc->FIFO.Channel[i];
				
				//Configure ADC Channels
				ADC->SC1 = cadc->FIFO.Channel[i];
			}
		}
		
		//Write ADC->SC1 to start Covertion
		ADC->SC1 |= (uint8_t)(cadc->Init.ConvMode);
	}
	else
	{
		//Disable ADC Pins Digital Logic
		ADC->APCTL1 = 1 << cadc->Init.Channel;
		
		//FIFO is Disabled, write ADC->SC1 to start Convertion
		ADC->SC1 = (uint8_t)(cadc->Init.ConvMode) | cadc->Init.Channel;
	}
}

/**
 * @brief	Set ADC Channel to start conversion
**/
static void ADC_SetChannel(ADC_HandleTypeDef* cadc)
{
	uint32_t tmpreg = ADC->SC1;
	tmpreg &= ~ADC_SC1_ADCH_MASK;
	ADC->SC1 = tmpreg | cadc->Init.Channel;
}
/**
 * Title 	ADC Extra Functions in CSL for KEAZxxx(Source File)
 * License	GPLv2.0
 * Author	Stark Zhang
 * Debug	None
**/

#include "KinetisKE_csl_adc_ex.h"

/**
 * @brief	Configure ADC as Temperature Sensor
 * @param	uint8_t ref
				Voltage Reference
				@arg	ADC_REF_ExtREF
				@arg	ADC_REF_VDDA
				@arg	ADC_REF_default
 * @return 	None
 * @note	this function will disable FIFO Mode
 *			if user need to customize ADC, you can use CSL_ADCEx_MspInit()
 *	 		user also can use Temperature Sensor by CSL_ADC_Init()
**/
void CSL_ADCEx_TemperatureSensor_Init(uint8_t ref)
{
	//Parameter Check
	assert_param(IS_ADC_REF(ref));
	
	//Enable the ADC Clock
	__CSL_ADC_CLK_ENABLE();
	
	//configure reference
	ADC->SC2 = ref;
	
	//configure clock source
	ADC->SC3 = ADC_SC3_ADLPC_MASK | 0x20u | ADC_SC3_ADLSMP_MASK | 0x40u | ADC_SC3_ADICLK_MASK;
	
	//other configuration
	ADC->SC4 = 0x00u;
	ADC->SC5 = 0x01u;
	
	//configure ADC_SC1 to start conversion
	ADC->SC1 = ADC_SC1_ADCO_MASK | ADC_Channel_T;
	
	//Msp Init
	CSL_ADCEx_MspInit();
}

/**
 * @brief	ADC Extra Function Msp Init
 * @param	None
 * @return 	None
 * @note	None
**/
__weak void CSL_ADCEx_MspInit(void)
{
	__no_operation();
}

/**
 * @brief	Get Temperature by ADC
 * @param	uint16_t Value
				result after conversion
 * @return 	float
				Temperature in Celsius
 * @note	Calculation formula is in DataSheet
 *			only for VDDA/VREF = 5.0V
**/
float CSL_ADCEx_GetTemperature(uint16_t Value)
{
	float temp = (float)Value * 5.0 / 4096.0;
	
	//Temperature > 25
	if(temp > 1.396)
	{
		temp = 25 - (temp - 1.396) / (3.266 * 10e-3);
	}
	//Temperature <= 25
	else 
	{
		temp = 25 - (temp - 1.396) / (3.638 * 10e-3);
	}
	
	return temp;
}

//EOF

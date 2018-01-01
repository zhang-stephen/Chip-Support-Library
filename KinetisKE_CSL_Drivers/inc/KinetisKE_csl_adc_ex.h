/**
 * Title 	ADC Extra Functions in CSL for KEAZxxx(Header File)
 * License	GPLv2.0
 * Author	Stark Zhang
 * Debug	None
**/

#ifndef __KinetisKE_CSL_ADC_EX_H
#define __KinetisKE_CSL_ADC_EX_H

#ifdef __cplusplus
 extern "C" {
#endif /*__cplusplus*/

#include "KinetisKE_csl_adc.h"

/* Functions for ADC Extra Functions */
void CSL_ADCEx_TemperatureSensor_Init(uint8_t ref);
void CSL_ADCEx_MspInit(void);
float CSL_ADCEx_GetTemperature(uint16_t Value);

#ifdef __cplusplus
 }
#endif /*__cplusplus*/

#endif /*__KinetisKE_CSL_ADC_EX_H*/

//EOF 

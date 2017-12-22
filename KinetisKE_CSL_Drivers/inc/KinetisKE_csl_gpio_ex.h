/**
 * Title 	FGPIO/Port module in CSL for KEAZ128(Header File)
 * License	GPLv2.0
 * Author	Stark Zhang
 * Debug	None
 * version	0.0.1 alpha
**/

/* Extra driver: This is for FGPIO & Port */

#ifndef __KinetisKE_CSL_GPIO_EX_H
#define __KinetisKE_CSL_GPIO_EX_H

#ifdef __cplusplus
 extern "C" {
#endif /*__cplusplus*/

#include "KinetisKE_csl_gpio.h"

/**
 * FGPIO Init-structure Type
**/
typedef struct
{
	uint32_t			Pin;							//GPIO_Pin
	uint8_t				Mode;							//GPIO_mode
	uint8_t 			Pull;							//Pullup or not(only for input)
}FGPIO_InitTypeDef;

/* Functions of FGPIO & Port */
void CSL_FGPIO_Init(FGPIO_Type* FGPIOx, FGPIO_InitTypeDef* FGPIO_Init);
void CSL_FGPIO_DeInit(FGPIO_Type* FGPIOx, uint32_t Pin);
void CSL_FGPIO_WritePin(FGPIO_MemMapPtr FGPIOx, uint32_t Pin, GPIO_LogicTypeDef logic);
void CSL_FGPIO_TogglePin(FGPIO_MemMapPtr FGPIOx, uint32_t Pin);
GPIO_LogicTypeDef CSL_FGPIO_ReadPin(FGPIO_MemMapPtr FGPIOx, uint32_t Pin);

/* Defgroup GPIO_FGPIO_PORT_Private_Macros CORTEX Private Macros */
#define IS_GPIO_FGPIOx(FGPIOx)			(((FGPIOx) == FGPIOA || ((FGPIOx) == FGPIOB) || ((FGPIOx) == FGPIOC)))

#ifdef __cplusplus
 }
#endif /*__cplusplus*/

#endif /*__KinetisKE_CSL_GPIO_EX_H*/

//EOF

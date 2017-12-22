/**
 * Title 	GPIO/Port module in CSL for KEAZ128(Header File)
 * License	GPLv2.0
 * Author	Stark Zhang
 * Debug	None
 * version	0.0.1 alpha
**/

/* Port & GPIO Modules, but not support to FGPIO */

#ifndef __KinetisKE_CSL_GPIO_H
#define __KinetisKE_CSL_GPIO_H

#ifdef __cplusplus
 extern "C" {
#endif /*__cplusplus*/

#include "KinetisKE_csl.h"

/**
 * GPIO Logic Level
**/
typedef enum
{
	GPIO_Logic_HiZ		= -1,			//High Impedance(HiZ)
	GPIO_Logic_0 		= 0,			//Logic Level 0(Low)
	GPIO_Logic_1 		= 1,			//Logic Level 1(High)
}GPIO_LogicTypeDef;

/**
 * GPIO Init-structure Type
**/
typedef struct
{
	uint32_t			Pin;							//GPIO_Pin
	uint8_t				Mode;							//GPIO_mode
	uint8_t 			Pull;							//Pullup or not(only for input)
}GPIO_InitTypeDef;

/**
 * GPIOA Pins
**/
#define GPIOA_Pin_PTA0			(1<<0)
#define GPIOA_Pin_PTA1			(1<<1)
#define GPIOA_Pin_PTA2			(1<<2)
#define GPIOA_Pin_PTA3			(1<<3)
#define GPIOA_Pin_PTA4			(1<<4)
#define GPIOA_Pin_PTA5			(1<<5)
#define GPIOA_Pin_PTA6			(1<<6)
#define GPIOA_Pin_PTA7			(1<<7)
#define GPIOA_Pin_PTB0			(1<<8)
#define GPIOA_Pin_PTB1			(1<<9)
#define GPIOA_Pin_PTB2			(1<<10)
#define GPIOA_Pin_PTB3			(1<<11)
#define GPIOA_Pin_PTB4			(1<<12)
#define GPIOA_Pin_PTB5			(1<<13)
#define GPIOA_Pin_PTB6			(1<<14)
#define GPIOA_Pin_PTB7			(1<<15)
#define GPIOA_Pin_PTC0			(1<<16)
#define GPIOA_Pin_PTC1			(1<<17)
#define GPIOA_Pin_PTC2			(1<<18)
#define GPIOA_Pin_PTC3			(1<<19)
#define GPIOA_Pin_PTC4			(1<<20)
#define GPIOA_Pin_PTC5			(1<<21)
#define GPIOA_Pin_PTC6			(1<<22)
#define GPIOA_Pin_PTC7			(1<<23)
#define GPIOA_Pin_PTD0			(1<<24)
#define GPIOA_Pin_PTD1			(1<<25)
#define GPIOA_Pin_PTD2			(1<<26)
#define GPIOA_Pin_PTD3			(1<<27)
#define GPIOA_Pin_PTD4			(1<<28)
#define GPIOA_Pin_PTD5			(1<<29)
#define GPIOA_Pin_PTD6			(1<<30)
#define GPIOA_Pin_PTD7			(1<<31)
#define GPIOA_Pin_ALL			0xFFFFFFFFU

/**
 * GPIOB Pins
**/
#define GPIOB_Pin_PTE0			(1<<0)
#define GPIOB_Pin_PTE1			(1<<1)
#define GPIOB_Pin_PTE2			(1<<2)
#define GPIOB_Pin_PTE3			(1<<3)
#define GPIOB_Pin_PTE4			(1<<4)
#define GPIOB_Pin_PTE5			(1<<5)
#define GPIOB_Pin_PTE6			(1<<6)
#define GPIOB_Pin_PTE7			(1<<7)
#define GPIOB_Pin_PTF0			(1<<8)
#define GPIOB_Pin_PTF1			(1<<9)
#define GPIOB_Pin_PTF2			(1<<10)
#define GPIOB_Pin_PTF3			(1<<11)
#define GPIOB_Pin_PTF4			(1<<12)
#define GPIOB_Pin_PTF5			(1<<13)
#define GPIOB_Pin_PTF6			(1<<14)
#define GPIOB_Pin_PTF7			(1<<15)
#define GPIOB_Pin_PTG0			(1<<16)
#define GPIOB_Pin_PTG1			(1<<17)
#define GPIOB_Pin_PTG2			(1<<18)
#define GPIOB_Pin_PTG3			(1<<19)
#define GPIOB_Pin_PTG4			(1<<20)
#define GPIOB_Pin_PTG5			(1<<21)
#define GPIOB_Pin_PTG6			(1<<22)
#define GPIOB_Pin_PTG7			(1<<23)
#define GPIOB_Pin_PTH0			(1<<24)
#define GPIOB_Pin_PTH1			(1<<25)
#define GPIOB_Pin_PTH2			(1<<26)
#define GPIOB_Pin_PTH3			(1<<27)
#define GPIOB_Pin_PTH4			(1<<28)
#define GPIOB_Pin_PTH5			(1<<29)
#define GPIOB_Pin_PTH6			(1<<30)
#define GPIOB_Pin_PTH7			(1<<31)
#define GPIOB_Pin_ALL			0xFFFFFFFFU

/**
 * GPIOC Pins
**/
#define GPIOC_Pin_PTI0			(1<<0)
#define GPIOC_Pin_PTI1			(1<<1)
#define GPIOC_Pin_PTI2			(1<<2)
#define GPIOC_Pin_PTI3			(1<<3)
#define GPIOC_Pin_PTI4			(1<<4)
#define GPIOC_Pin_PTI5			(1<<5)
#define GPIOC_Pin_PTI6			(1<<6)
#define GPIOC_Pin_PTI7			(1<<7)
#define GPIOC_Pin_ALL			0x000000FFU

/**
 * GPIO Mode
**/
#define GPIO_Mode_Input         0x00U
#define GPIO_Mode_Output        0x01U

/**
 * GPIO pull-up or no-pull(without internal pull-down)
**/
#define GPIO_PUPD_UP            0x00U 
#define GPIO_PUPD_NOPULL        0x01U

/**
 * High Current Pins
**/
#define GPIO_HDRVE_PTH1			((uint32_t)(0x80U))
#define GPIO_HDRVE_PTH0			((uint32_t)(0x40U))
#define GPIO_HDRVE_PTE1			((uint32_t)(0x20U))
#define GPIO_HDRVE_PTE0			((uint32_t)(0x10U))
#define GPIO_HDRVE_PTD1			((uint32_t)(0x08U))
#define GPIO_HDRVE_PTD0			((uint32_t)(0x04U))
#define GPIO_HDRVE_PTB5			((uint32_t)(0x02U))
#define GPIO_HDRVE_PTB4			((uint32_t)(0x01U))
#define GPIO_HDRVE_ALL			((uint32_t)(0xFFU))

/* Functions of GPIO & Port */
void CSL_GPIO_Init(GPIO_Type* GPIOx, GPIO_InitTypeDef* GPIO_Init);
void CSL_GPIO_DeInit(GPIO_Type* GPIOx, uint32_t Pin);
void CSL_GPIO_WritePin(GPIO_MemMapPtr GPIOx, uint32_t Pin, GPIO_LogicTypeDef logic);
void CSL_GPIO_TogglePin(GPIO_MemMapPtr GPIOx, uint32_t Pin);
GPIO_LogicTypeDef CSL_GPIO_ReadPin(GPIO_MemMapPtr GPIOx, uint32_t Pin);

//High Current Drive, active to FGPIO
void CSL_GPIO_EnableHDRVE(uint32_t GPIO_HDRVE_Pin);
void CSL_GPIO_DisableHDRVE(uint32_t GPIO_HDRVE_Pin);

//Port Configure
void CSL_PORT_MspInit(void);
void CSL_PORT_MspDeInit(void);

/* Defgroup GPIO_FGPIO_PORT_Private_Macros CORTEX Private Macros */
#define IS_GPIO_GPIOx(GPIOx)			(((GPIOx) == GPIOA || ((GPIOx) == GPIOB) || ((GPIOx) == GPIOC)))
#define IS_GPIO_Logic(logic)			(((logic) == GPIO_Logic_HiZ) || ((logic) == GPIO_Logic_0) || ((logic) == GPIO_Logic_1))
#define IS_GPIO_HDRVE(hdrve)			((hdrve <= GPIO_HDRVE_ALL) || (hdrve > 0x00U))

#ifdef __cplusplus
 }
#endif /*__cplusplus*/

#endif /*__KinetisKE_CSL_GPIO_H*/

//EOF

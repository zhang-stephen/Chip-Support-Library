/**
 * Title 	Csl Configuration for KinetisKE MCUs Header File
 * License	GPLv2.0
 * Author	Stark Zhang
 * Debug	None
**/

#ifndef __KinetisKE_CSL_CONFIG_H
#define __KinetisKE_CSL_CONFIG_H

#ifdef __cplusplus
 extern "C" {
#endif /*__cplusplus*/

/**
 * SysTick NVIC Priority
**/
#define Tick_Int_Priority		((uint32_t)(0x00u))

/* Definations for Flash Module */
#define FLASH_ADDR_BASE			((uint32_t)(0x00000000U))		//Flash Address Base
#define FLASH_ADDR_TOP			((uint32_t)(0x0001FFFFU))		//Flash Address Top

/* Clock Control Bits */
/**
 * @brief 	Enable/Disable ICS Interrupt
 * @arg		0x00u	ICS Interrupt Disabled
			0x01u	ICS Interrput Enabled
**/
#define	ICS_EXTI_STAT			0x01u

/* Flash Control Bits */
/**
 * @brief	Enable/Disable Flash Speclated
 * @arg		0x00u	Speculated --> only instruction
			0x01u	Speculated --> instruction & data
			0x10u	Speculated --> Disabled
			0x11u	as same as 0x10
**/
#define FLASH_SPECULATED_STAT	0x01u

/**
 * @brief	Enable/Disable Flash High-Speed Cache
 * @arg		0x000u	Cache --> instruction & data & control
			0x001u	Cache --> instruction & control
			0x010u	Cache --> data & control
			0x011u	Cache --> control
			0x1yyu	Cache --> completed off(y = don't care)
			others 	Cache --> completed off
**/
#define FLASH_CACHE_STAT		0x000u

/**
 * @brief	Enable/Disable Flash IRQn(FTMRE_IRQHandle)
 * @arg		0x00U	Flash IRQn Disabled
 			0x01U	Flash IRQn Enabled
**/
#define FLASH_EXTI_STAT			0x00u

/**
 * System Clock Gate Control(enable/disbale module clocks) SIM->SCGC
**/
#define __CSL_ACMP1_CLK_ENABLE()            (SIM->SCGC |= SIM_SCGC_ACMP1_MASK)
#define __CSL_ACMP0_CLK_ENABLE()            (SIM->SCGC |= SIM_SCGC_ACMP0_MASK)
#define __CSL_ADC_CLK_ENABLE()              (SIM->SCGC |= SIM_SCGC_ADC_MASK)
#define __CSL_IRQ_CLK_ENABLE()              (SIM->SCGC |= SIM_SCGC_IRQ_MASK)
#define __CSL_KBI1_CLK_ENABLE()             (SIM->SCGC |= SIM_SCGC_KBI1_MASK)
#define __CSL_KBI0_CLK_ENABLE()             (SIM->SCGC |= SIM_SCGC_KBI0_MASK)
#define __CSL_UART2_CLK_ENABLE()            (SIM->SCGC |= SIM_SCGC_UART2_MASK)
#define __CSL_UART1_CLK_ENABLE()            (SIM->SCGC |= SIM_SCGC_UART1_MASK)
#define __CSL_UART0_CLK_ENABLE()            (SIM->SCGC |= SIM_SCGC_UART0_MASK)
#define __CSL_SPI1_CLK_ENABLE()             (SIM->SCGC |= SIM_SCGC_SPI1_MASK)
#define __CSL_SPI0_CLK_ENABLE()             (SIM->SCGC |= SIM_SCGC_SPI0_MASK)
#define __CSL_I2C1_CLK_ENABLE()             (SIM->SCGC |= SIM_SCGC_I2C1_MASK)
#define __CSL_I2C0_CLK_ENABLE()             (SIM->SCGC |= SIM_SCGC_I2C0_MASK)
#define __CSL_MSCAN_CLK_ENABLE()            (SIM->SCGC |= SIM_SCGC_MSCAN_MASK)
#define __CSL_SWD_CLK_ENABLE()              (SIM->SCGC |= SIM_SCGC_SWD_MASK)
#define __CSL_FLASH_CLK_ENABLE()            (SIM->SCGC |= SIM_SCGC_FLASH_MASK)
#define __CSL_CRC_CLK_ENABLE()              (SIM->SCGC |= SIM_SCGC_CRC_MASK)
#define __CSL_FTM2_CLK_ENABLE()             (SIM->SCGC |= SIM_SCGC_FTM2_MASK)
#define __CSL_FTM1_CLK_ENABLE()             (SIM->SCGC |= SIM_SCGC_FTM1_MASK)
#define __CSL_FTM0_CLK_ENABLE()             (SIM->SCGC |= SIM_SCGC_FTM0_MASK)
#define __CSL_PWT_CLK_ENABLE()              (SIM->SCGC |= SIM_SCGC_PWT_MASK)
#define __CSL_PIT_CLK_ENABLE()              (SIM->SCGC |= SIM_SCGC_PIT_MASK)
#define __CSL_RTC_CLK_ENABLE()              (SIM->SCGC |= SIM_SCGC_RTC_MASK)

#define __CSL_ACMP1_CLK_DISABLE()            (SIM->SCGC &= ~SIM_SCGC_ACMP1_MASK)
#define __CSL_ACMP0_CLK_DISABLE()            (SIM->SCGC &= ~SIM_SCGC_ACMP0_MASK)
#define __CSL_ADC_CLK_DISABLE()              (SIM->SCGC &= ~SIM_SCGC_ADC_MASK)
#define __CSL_IRQ_CLK_DISABLE()              (SIM->SCGC &= ~SIM_SCGC_IRQ_MASK)
#define __CSL_KBI1_CLK_DISABLE()             (SIM->SCGC &= ~SIM_SCGC_KBI1_MASK)
#define __CSL_KBI0_CLK_DISABLE()             (SIM->SCGC &= ~SIM_SCGC_KBI0_MASK)
#define __CSL_UART2_CLK_DISABLE()            (SIM->SCGC &= ~SIM_SCGC_UART2_MASK)
#define __CSL_UART1_CLK_DISABLE()            (SIM->SCGC &= ~SIM_SCGC_UART1_MASK)
#define __CSL_UART0_CLK_DISABLE()            (SIM->SCGC &= ~SIM_SCGC_UART0_MASK)
#define __CSL_SPI1_CLK_DISABLE()             (SIM->SCGC &= ~SIM_SCGC_SPI1_MASK)
#define __CSL_SPI0_CLK_DISABLE()             (SIM->SCGC &= ~SIM_SCGC_SPI0_MASK)
#define __CSL_I2C1_CLK_DISABLE()             (SIM->SCGC &= ~SIM_SCGC_I2C1_MASK)
#define __CSL_I2C0_CLK_DISABLE()             (SIM->SCGC &= ~SIM_SCGC_I2C0_MASK)
#define __CSL_MSCAN_CLK_DISABLE()            (SIM->SCGC &= ~SIM_SCGC_MSCAN_MASK)
#define __CSL_SWD_CLK_DISABLE()              (SIM->SCGC &= ~SIM_SCGC_SWD_MASK)
#define __CSL_FLASH_CLK_DISABLE()            (SIM->SCGC &= ~SIM_SCGC_FLASH_MASK)
#define __CSL_CRC_CLK_DISABLE()              (SIM->SCGC &= ~SIM_SCGC_CRC_MASK)
#define __CSL_FIM2_CLK_DISABLE()             (SIM->SCGC &= ~SIM_SCGC_FTM2_MASK)
#define __CSL_FIM1_CLK_DISABLE()             (SIM->SCGC &= ~SIM_SCGC_FTM1_MASK)
#define __CSL_FIM0_CLK_DISABLE()             (SIM->SCGC &= ~SIM_SCGC_FTM0_MASK)
#define __CSL_PWT_CLK_DISABLE()              (SIM->SCGC &= ~SIM_SCGC_PWT_MASK)
#define __CSL_PIT_CLK_DISABLE()              (SIM->SCGC &= ~SIM_SCGC_PIT_MASK)
#define __CSL_RTC_CLK_DISABLE()              (SIM->SCGC &= ~SIM_SCGC_RTC_MASK)

#ifdef __cplusplus
 }
#endif /*__cplusplus*/
 
#endif /*__KinetisKE_CSL_CONFIG_H*/

//EOF

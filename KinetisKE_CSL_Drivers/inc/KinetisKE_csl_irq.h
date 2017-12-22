/**
 * Title 	IRQ module in CSL for KEAZ128(Header File)
 * License	GPLv2.0
 * Author	Stark Zhang
 * Debug	None
**/

/* Just for 1 external input interrupt(which can be shiled) */

#ifndef __KinetisKE_CSL_IRQ_H
#define __KinetisKE_CSL_IRQ_H

#ifdef __cplusplus
 extern "C" {
#endif /*__cplusplus*/

#include "KinetisKE_csl.h"

/**
 * Type re-define for IRQ Init 
**/
typedef struct
{
	uint8_t IRQ_Pin;			/* for IRQ Pins Selection(just one pin) */
	uint8_t IRQ_Pull;			/* for IRQ Pull-up or not */
	uint8_t Trigger;			/* for Trigger Source is Rising Edge/High-level or Falling Edge/Low-level */
	uint8_t IRQ_Mode;			/* for Detection on only edge or both edge and level */
}IRQ_InitTypeDef;

/**
 * IRQ Pins Defination(SIM->PINSEL0[IRQPS])
**/
#define IRQ_Pin_PTA5			0x00u
#define IRQ_Pin_PTI0			0x01u
#define IRQ_Pin_PTI1			0x02u
#define IRQ_Pin_PTI2			0x03u
#define IRQ_Pin_PTI3			0x04u
#define IRQ_Pin_PTI4			0x05u
#define IRQ_Pin_PTI5			0x06u
#define IRQ_Pin_PTI6			0x07u
#define IRQ_Pin_MASK			SIM_PINSEL_IRQPS_MASK

/**
 * IRQ Pull-up Defination
**/
#define IRQ_Int_Pullup			0x00u
#define IRQ_Int_Nopull			IRQ_SC_IRQPDD_MASK

/**
 * IRQ Trigger
**/
#define	IRQ_Trig_Rising			IRQ_SC_IRQEDG_MASK
#define IRQ_Trig_Falling		0x00u

/**
 * IRQ Detection Mode
**/
#define IRQ_Only_Edge			0x00u
#define IRQ_Edge_Level			IRQ_SC_IRQMOD_MASK 

/* Macros Functions for IRQ */
/**
 * @brief 	Enable/Disable IRQ
**/
#define __IRQ_ENABLE()			(IRQ->SC |= IRQ_SC_IRQPE_MASK)
#define __IRQ_DISABLE()			(IRQ->SC &= ~IRQ_SC_IRQPE_MASK)

/**
 * @brief	Enable/Disable IRQ Interrupt
**/
#define __IRQ_EXTI_ENABLE()		(IRQ->SC |= IRQ_SC_IRQIE_MASK)
#define __IRQ_EXTI_DISABLE()	(IRQ->SC &= ~IRQ_SC_IRQIE_MASK)

/**
 * @brief	Clear IRQ Interrupt Flags
**/
#define __IRQ_EXTI_CLEAR_FLAG()	(IRQ->SC |= IRQ_SC_IRQACK_MASK)

/* Public Functions for IRQ */
void CSL_IRQ_Init(IRQ_InitTypeDef* IRQ_Init);
void CSL_IRQ_DeInit(void);

/* Interrupt Functions */
void CSL_IRQ_IRQHandler(void);
void CSL_IRQ_Callback(void);

/* Defgroup IRQ_Private_Macros CORTEX Private Macros */
#define IS_IRQ_Pins(pin)		(pin <= IRQ_Pin_PTI6)
#define IS_IRQ_Pullup(x)		((x == IRQ_Int_Pullup) || (x == IRQ_Int_Nopull))
#define IS_IRQ_Trig(trig)		((trig == IRQ_Trig_Rising) || (trig == IRQ_Trig_Falling))
#define IS_IRQ_Mode(mode) 		((mode == IRQ_Only_Edge) || (mode == IRQ_Edge_Level))

#ifdef __cplusplus
 }
#endif /*__cplusplus*/

#endif /*__KinetisKE_CSL_IRQ_H*/

//EOF

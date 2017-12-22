/**
 * Title 	IRQ module in CSL for KEAZ128(Source File)
 * License	GPLv2.0
 * Author	Stark Zhang
 * Debug	None
**/

#include "KinetisKE_csl_irq.h"

/**
 * @brief   Initialize IRQ
 * @param   IRQ_InitTypeDef* IRQ_Init
                IRQ initialized structure
 * @return  None
 * @note    None
**/
void CSL_IRQ_Init(IRQ_InitTypeDef* IRQ_Init)
{
    //Parameter Check
	assert_param(IS_IRQ_Pins(IRQ_Init->IRQ_Pin));
    assert_param(IS_IRQ_Pullup(IRQ_Init->IRQ_Pull));
    assert_param(IS_IRQ_Trig(IRQ_Init->Trigger));
    assert_param(IS_IRQ_Mode(IRQ_Init->IRQ_Mode));
    
    //Pin Selection
    SIM->PINSEL &= ~IRQ_Pin_MASK;
    SIM->PINSEL |= IRQ_Init->IRQ_Pin;
    
    //Enable IRQ & its EXTI
    __IRQ_ENABLE();                 //Enable IRQ
    __IRQ_EXTI_ENABLE();            //Enable IRQ Interrupt
    
    //Settings
    IRQ->SC |= (IRQ_Init->IRQ_Pull << IRQ_SC_IRQPDD_SHIFT) | 
               (IRQ_Init->Trigger << IRQ_SC_IRQEDG_SHIFT) | 
               (IRQ_Init->IRQ_Mode << IRQ_SC_IRQMOD_SHIFT);
}

/**
 * @brief   Initialize IRQ
 * @param   None
 * @return  None
 * @note    None
**/
void CSL_IRQ_DeInit(void)
{
    //Diable IRQ & its EXTI
    __IRQ_DISABLE();
    __IRQ_EXTI_DISABLE();
    __IRQ_EXTI_CLEAR_FLAG();
    
    //IRQ Status & Control Register Reset
    IRQ->SC = 0x00u;
    
    //Pin Selection Reset
    SIM->PINSEL &= ~IRQ_Pin_MASK;
}

/**
 * @brief   CSL_IRQ_Handler, Called by IRQ_IRQHandler()
**/
void CSL_IRQ_IRQHandler(void)
{
    CSL_IRQ_Callback();
    //Clear Flag
    while((IRQ->SC & IRQ_SC_IRQF_MASK) >> IRQ_SC_IRQF_SHIFT != 0)
    {
        __IRQ_EXTI_CLEAR_FLAG();
    }
}

/**
 * @brief   IRQ_IRQHandler() User Callback Function
 * @note    Do not modify this function, user should re-define it in other files
**/
__weak void CSL_IRQ_Callback(void)
{
    __no_operation();
}

//EOF

/**
 * Title 	Cortex-M0+ module in CSL for KEAZ128(Source File)
 * License	GPLv2.0
 * Author	Stark Zhang
 * Debug	None
**/

/* KEA is not supported to MPU & FPU, only for NVIC & Systick */

#include "KinetisKE_csl_cortex.h"

/* Initialization and de-initialization functions */

/**
 * @brief   CSL NVIC priority set
 * @param   IRQn_Type IRQn
                The IRQn will be set
 * @return  None
 * @note    None
**/
void CSL_NVIC_SetPriority(IRQn_Type IRQn, uint32_t priority)
{
    assert_param(IS_NVIC_Device_IRQn(IRQn));
    NVIC_SetPriority(IRQn, priority);
}

/**
 * @brief   CSL NVIC Enable specific IRQn
 * @param   IRQn_Type IRQn
                The IRQn will be set
 * @return  None
 * @note    None
**/
void CSL_NVIC_EnableIRQ(IRQn_Type IRQn)
{
    assert_param(IS_NVIC_Device_IRQn(IRQn));
    NVIC_EnableIRQ(IRQn);
}

/**
 * @brief   CSL NVIC Disable specific IRQn
 * @param   IRQn_Type IRQn
                The IRQn will be set
 * @return  None
 * @note    None
**/
void CSL_NVIC_DisableIRQ(IRQn_Type IRQn)
{
    assert_param(IS_NVIC_Device_IRQn(IRQn));
    NVIC_DisableIRQ(IRQn);
}

/**
 * @brief   CSL NVIC System Reset
 * @param   None
 * @return  None
 * @note    None
**/
void CSL_NVIC_SystemReset(void)
{
    NVIC_SystemReset();
}

/**
 * @brief   CSL Systick Configure
 * @param   None
 * @return  uint32_t
				@arg = 1	failed
				@arg = 0	successed
 * @note    None
**/
uint32_t CSL_SYSTICK_Config(uint32_t TickThumb)
{
	return SysTick_Config(TickThumb);
}

/**
 * @brief   CSL Systick Clock Source Configure
 * @param   uint32_t TickClockSource
				Tick Pre-Scaler
				@arg SysTick_CLKSource_CoreCLK_DIV16 Divider = 16
				@arg SysTick_CLKSource_CoreCLK		 Divider = 1
 * @return  None
 * @note    None
**/
void CSL_SYSTICK_ClockSourceConfig(uint32_t TickClockSource)
{
	//Parameter Check
	assert_param(IS_SYSTICK_CLKSource(TickClockSource));
	
	//Tick Frequency = CoreCLK / 16
	if(TickClockSource == SysTick_CLKSource_CoreCLK_DIV16)
	{
		SysTick->CTRL &= ~SysTick_CTRL_CLKSOURCE_Msk;
	}
	//Tick Frequency = CoreCLK
	else 
	{
		SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
	}
}

/**
 * @brief	SYSTICK_IRQHandler ISR function
 * @note	called by SYSTICK_IRQHandler()
**/
void CSL_SYSTICK_IRQHandler(void)
{
	CSL_IncTick();
	CSL_SYSTICK_Callback();
}

/**
 * @brief	SYSTICK_IRQHandler User-defined function
 * @note	do not modify this fucntion, but you can rewrite it in other file
**/
__weak void CSL_SYSTICK_Callback(void)
{
	UNUSED(0);
}

/* NVIC Functions */

/**
  * @brief  Gets Pending Interrupt (reads the pending register in the NVIC 
  *         and returns the pending bit for the specified interrupt).
  * @param  IRQn External interrupt number.
  *          This parameter can be an enumerator of IRQn_Type enumeration
  *         (For the complete KinetisKE Devices IRQ Channels list, please refer to the appropriate CMSIS device file)
  * @retval status: - 0  Interrupt status is not pending.
  *                 - 1  Interrupt status is pending.
  */
uint32_t CSL_NVIC_GetPendingIRQ(IRQn_Type IRQn)
{
	assert_param(IS_NVIC_Device_IRQn(IRQn));
	return NVIC_GetPendingIRQ(IRQn);
}

/**
  * @brief  Sets Pending bit of an external interrupt.
  * @param  IRQn External interrupt number
  *         This parameter can be an enumerator of IRQn_Type enumeration
  *         (For the complete KinetisKE Devices IRQ Channels list, please refer to the appropriate CMSIS device file)
  * @retval None
  */
void CSL_NVIC_SetPendingIRQ(IRQn_Type IRQn)
{
	assert_param(IS_NVIC_Device_IRQn(IRQn));
	NVIC_SetPendingIRQ(IRQn);
}

/**
  * @brief  Clears the pending bit of an external interrupt.
  * @param  IRQn External interrupt number.
  *         This parameter can be an enumerator of IRQn_Type enumeration
  *         (For the complete KinetisKE Devices IRQ Channels list, please refer to the appropriate CMSIS device file)
  */
void CSL_NVIC_ClearPendingIRQ(IRQn_Type IRQn)
{
	assert_param(IS_NVIC_Device_IRQn(IRQn));
	NVIC_ClearPendingIRQ(IRQn);
}

//EOF

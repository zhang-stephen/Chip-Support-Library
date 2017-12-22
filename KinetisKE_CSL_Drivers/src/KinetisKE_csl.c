/**
 * Title 	Chip Support Library for KinetisKE MCUs Source File
 * License	GPLv2.0
 * Author	Stark Zhang
 * Debug	None
**/

#include "KinetisKE_csl.h"
#include "KinetisKE_csl_clk.h"
#include "KinetisKE_csl_cortex.h"
#include "KinetisKE_csl_flash.h"

/**
 * @brief 	recording count of Tick
**/
static uint32_t uwTick = 0;

/**
 * @brief   Initiatilize All of Chip
 * @param   None
 * @return  None
 * @note    None
**/
__weak CSL_StatusTypeDef CSL_Init(void)
{
    /*SWD CLock Enable */
    __CSL_SWD_CLK_ENABLE();
    
	/* Configure Clock/ICS */
#if (ICS_EXTI_STAT == 0x01u)
	__ICS_ENABLE_EXTI();
	CSL_NVIC_EnableIRQ(ICS_IRQn);
	CSL_NVIC_SetPriority(ICS_IRQn, 0);
#else
	__ICS_DISABLE_EXTI();
#endif /*ICS_EXTI_STAT == 0x01u*/
	
	/* Init Flash */
	//Enable Speculated or not
#if (FLASH_SPECULATED_STAT == 0x00u)
	__FLASH_ENABLE_ESFC();
	__FLASH_DISABLE_DFCS();
#elif (FLASH_SPECULATED_STAT == 0x01)
	__FLASH_ENABLE_ESFC();
	__FLASH_ENABLE_DFCS();
#else
	__FLASH_DISABLE_ESFC();
	__FLASH_DISABLE_DFCS();
#endif /*FLASH_SPECULATED_STAT*/
	
	//Enable Cache or not
	__FLASH_CLEAR_CONTROL_CACHE();
#if (FLASH_CACHE_STAT == 0x000u)
	__FLASH_ENABLE_DFCC();
	__FLASH_ENABLE_DFCIC();
	__FLASH_ENABLE_DFCDA();
#elif (FLASH_CACHE_STAT == 0x001u)
	__FLASH_ENABLE_DFCC();
	__FLASH_ENABLE_DFCIC();
	__FLASH_DISABLE_DFCDA();
#elif (FLASH_CACHE_STAT == 0x010u)
	__FLASH_ENABLE_DFCC();
	__FLASH_DISABLE_DFCIC();
	__FLASH_ENABLE_DFCDA();
#elif (FLASH_CACHE_STAT == 0x011u)
	__FLASH_ENABLE_DFCC();
	__FLASH_DISABLE_DFCIC();
	__FLASH_DISABLE_DFCDA();
#else
	__FLASH_DISABLE_DFCC();
	__FLASH_DISABLE_DFCIC();
	__FLASH_DISABLE_DFCDA();
#endif /*FLASH_CACHE_STAT*/
	
	//Enable Interrupt or not
#if (FLASH_EXTI_STAT == 0x01u)
	__FLASH_ENABLE_EXTI();
	CSL_NVIC_EnableIRQ(FTMRE_IRQn);
	CSL_NVIC_SetPriority(FTMRE_IRQn, 0);
#else
	__FLASH_DISABLE_EXTI();
#endif /*FLASH_EXTI_STAT == 0x01U*/
	
	//Init Int Flash
	if(CSL_Flash_Init() != CSL_OK)
		return CSL_Error;

	return CSL_OK;
}

/**
 * @brief	User-defined Chip Initialize
 * @param	None
 * @return	None
 * @note	Do not modify this file, user should re-define it in user's code
**/
__weak void CSL_MspInit(void)
{
	//Enable RESET & NMI pins
	SIM->SOPT0 |= (uint32_t)0x60u;
	
	//System Interrupts Setting
	CSL_NVIC_SetPriority(NonMaskableInt_IRQn, 0);
	CSL_NVIC_SetPriority(HardFault_IRQn, 0);
	CSL_NVIC_SetPriority(SVCall_IRQn, 0);
	CSL_NVIC_SetPriority(PendSV_IRQn, 0);
}

__weak CSL_StatusTypeDef CSL_InitTick(uint32_t TickPriority)
{		
	/* Configure the SysTick to have interrupt in 1ms time basis */
	CSL_SYSTICK_Config(SystemCoreClock / 1000);
	
	/* Configure the SysTick IRQ priority */
	CSL_NVIC_SetPriority(SysTick_IRQn, TickPriority);
	
	/* return CSL status */
	return CSL_OK;
}

/**
 * @brief   calculate Count of Tick
 * @param   None
 * @return  None
 * @note    None
**/
__weak void CSL_IncTick(void)
{
	uwTick++;
}

/**
 * @brief   Get Count of Tick
 * @param   None
 * @return  None
 * @note    None
**/
__weak uint32_t CSL_GetTick(void)
{
	return uwTick;
}

/**
 * @brief   Suspend Systick
 * @param   None
 * @return  None
 * @note    None
**/
void CSL_SuspendTick(void)
{
	/* Disable SysTick Interrupt */
	SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk;
}

/**
 * @brief   Resume Systick after Suspending
 * @param   None
 * @return  None
 * @note    None
**/
void CSL_ResumeTick(void)
{
	/* Enable SysTick Interrupt */
  	SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk;
}

/**
 * @brief   CSL Delay function
 * @param   __IO uint32_t Delay
				Tick period will be delayed
 * @return  None
 * @note    None
**/
__weak void CSL_Delay(__IO uint32_t Delay)
{
	uint32_t tickstart = CSL_GetTick();
	uint32_t wait = Delay;
	
	/* Add a period to guarantee minimum wait */
	if (wait < CSL_MAX_DELAY)
	{
		wait++;
	}
	
	while((CSL_GetTick() - tickstart) < wait)
	{
	}
}

/**
 * @brief 	Get Device ID 
 * @param	None
 * @return 	uint16_t --> Device ID
 * @note	None
**/
uint16_t CSL_GetDevID(void)
{
	return ((uint16_t)(SIM->SRSID >> 16));
}

/**
 * @brief 	Get unique ID 
 * @param	uint32_t* uid
				Buffer to store the UID
 * @return 	None
 * @note	This UID is 80-bit binary number
**/
void CSL_GetUID(uint32_t *uid)
{
	*(uid+0) = SIM->UUIDL;
	*(uid+1) = SIM->UUIDML;
	*(uid+2) = SIM->UUIDMH;
}

//EOF

/**
 * Title 	Power Management Controller module in CSL for KEAZ128(Source File)
 * License	GPLv2.0
 * Author	Stark Zhang
 * Debug	None
**/

#include "KinetisKE_csl_pmc.h"

/**
 * @brief	Initialize PMC/LVD
 * @param	PMC_HandleTypeDef* cpmc
				PMC Handle
 * @return	CSL_StatusTypeDef
 * @note	if LVD is initialized, it cannot be disbaled except rewrite code!
**/
CSL_StatusTypeDef CSL_PMC_LVD_Init(PMC_HandleTypeDef* cpmc)
{
	//Parameter Check
	assert_param(IS_LVD_VDL(cpmc->Init.VDL));
	
	if(cpmc == NULL)
	{
		cpmc->ErrorCode = CSL_PMC_ERROR_CONFIG;
		return CSL_Error;
	}
	
	/* Get last POR State */
	//No POR
	if((SIM->SRSID & SIM_SRSID_POR_MASK >> SIM_SRSID_POR_SHIFT) == 0)
	{
		cpmc->LVD_State = CSL_LVD_STATE_NONE;
	}
	//No Low-Voltage Warning
	else if((SIM->SRSID & SIM_SRSID_LVD_MASK >> SIM_SRSID_LVD_SHIFT) == 0)
	{
		cpmc->LVD_State = CSL_LVD_STATE_POR;
	}
	//POR caused by Low-Voltage Warning
	else
	{
		cpmc->LVD_State = CSL_LVD_STATE_LVWPOR;
	}
	
	//Write PMC Registers
	PMC->SPMSC2 |= cpmc->Init.VDL;
	if(cpmc->Init.Range == LVD_Range_Low)
		CLEAR_BIT(PMC->SPMSC2, PMC_SPMSC2_LVDV_MASK);
	else SET_BIT(PMC->SPMSC2, PMC_SPMSC2_LVDV_MASK);
	
	//Enable LVD(excluding STOP mode)
	SET_BIT(PMC->SPMSC1, PMC_SPMSC1_LVDE_MASK);
	
	//User Init
	CSL_PMC_LVD_MspInit();
	
	cpmc->ErrorCode = CSL_PMC_ERROR_NONE;
	return CSL_OK;
}

/**
 * @brief	PMC/LVD user Init
 * @param	None
 * @return	None
 * @note	if user want to adjust PMC/LVD configuration, should rewrite this function in user code!
**/
__weak void CSL_PMC_LVD_MspInit(void)
{
	//Enable LVD interrupt
	__PMC_LVD_EXTI_ENABLE();
	__PMC_LVD_CLEAR_FLAG();
	
	NVIC_EnableIRQ(PMC_IRQn);
	NVIC_SetPriority(PMC_IRQn, 0);
	
	//Enable LVD in STOP mode
	SET_BIT(PMC->SPMSC1, PMC_SPMSC1_LVDSE_MASK);
}

/**
 * @brief	PMC/LVD user DeInit
 * @param	None
 * @return	None
 * @note	if user want to adjust PMC/LVD configuration, should rewrite this function in user code!
**/
__weak void CSL_PMC_LVD_MspDeInit(void)
{
	__no_operation();
}

/**
 * @brief	Get LVD/POR State since last boot
 * @param	PMC_HandleTypeDef* cpmc
				PMC Handle
 * @return	uint8_t
 * @note	None
**/
uint8_t CSL_PMC_GetLVDState(PMC_HandleTypeDef* cpmc)
{
	return cpmc->LVD_State;
}

/**
 * @brief	Get PMC Error Code
 * @param	PMC_HandleTypeDef* cpmc
				PMC Handle
 * @return	uint8_t
 * @note	None
**/
uint8_t CSL_PMC_GetError(PMC_HandleTypeDef* cpmc)
{
	return cpmc->ErrorCode;
}

/**
 * @brief	Step into Wait Mode(low-power)
 * @param	uint8_t LowPowerEntry
				@arg PMC_LowPower_Entry_WFI __WFI()
				@arg PMC_LowPower_Entry_WFE __WFE()
 * @return	None
 * @note	None
**/
void CSL_PMC_EnterWaitMode(uint8_t LowPowerEntry)
{
	/* Clear SLEEPDEEP bit of Cortex System Control Register */
	CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));

	/* Select SLEEP mode entry -------------------------------------------------*/
	if(LowPowerEntry == PMC_LowPower_Entry_WFI)
	{	
		/* Request Wait For Interrupt */
		__WFI();
	}
	else
	{
		/* Request Wait For Event */
		__SEV();
		__WFE();
		__WFE();
	}
}

/**
 * @brief	Step into STOP Mode(low-power)
 * @param	uint8_t LowPowerEntry
				@arg PMC_LowPower_Entry_WFI __WFI()
				@arg PMC_LowPower_Entry_WFE __WFE()
 * @return	None
 * @note	None
**/
void CSL_PMC_EnterStopMode(uint8_t LowPowerEntry)
{
	/* Clear SLEEPDEEP bit of Cortex System Control Register */
	SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));

	/* Select SLEEP mode entry -------------------------------------------------*/
	if(LowPowerEntry == PMC_LowPower_Entry_WFI)
	{	
		/* Request Wait For Interrupt */
		__WFI();
	}
	else
	{
		/* Request Wait For Event */
		__SEV();
		__WFE();
		__WFE();
	}
	CLEAR_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));
}

/**
 * @brief	Step into STANDBY Mode(low-power)
 * @param	uint8_t LowPowerEntry
				@arg PMC_LowPower_Entry_WFI __WFI()
				@arg PMC_LowPower_Entry_WFE __WFE()
 * @return	None
 * @note	None
**/
void CSL_PMC_EnterStandByMode(uint8_t LowPowerEntry)
{
	/* Set SLEEPDEEP bit of Cortex System Control Register */
	SET_BIT(SCB->SCR, ((uint32_t)SCB_SCR_SLEEPDEEP_Msk));
  
	/* This option is used to ensure that store operations are completed */
#if defined ( __CC_ARM)
	__force_stores();
#endif
	/* Request Wait For Interrupt */
	__WFI();
}

/**
 * @brief	PMC IRQHandler in CSL
 * @param	PMC_HandleTypeDef* cpmc
				PMC Handle
 * @return	None
 * @note	None
**/
void CSL_PMC_IRQHandler(PMC_HandleTypeDef* cpmc)
{
	//Clear LVD Flags
	__PMC_LVD_CLEAR_FLAG();
	
	//record POR
	cpmc->LVD_State = __PMC_LVD_CLEAR_FLAG();
	
	//User Callback
	CSL_PMC_LVD_Callback();
}

/**
 * @brief	PMC_IRQHandler User Callback 
 * @param	None
 * @return	None
 * @note	do not modify this function
**/
__weak void CSL_PMC_LVD_Callback(void)
{
	__no_operation();
}

//EOF

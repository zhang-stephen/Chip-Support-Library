/**
 * Title 	PWT module in CSL for KEAZ128(Source File)
 * License	GPLv2.0
 * Author	Stark Zhang
 * Debug	None
**/

#include "KinetisKE_csl_pwt.h"

/**
 * @brief
 * @param
 * @return
 * @note
**/
CSL_StatusTypeDef CSL_PWT_Init(PWT_HandleTypeDef* cpwt)
{
	//Parameters Check
	if(cpwt == NULL)
	{
		return CSL_Error;
	}
	assert_param(IS_PWT_CLKSource(cpwt->Init.ClockSource));
	assert_param(IS_PWT_CLKSourceDIV(cpwt->Init.PreScaler));
	assert_param(IS_PWT_EDGE(cpwt->Init.Edge));
	assert_param(IS_PWT_INx(cpwt->Init.Inx));
	
	//Soft Reset
	PWT->R1 = PWT_R1_PWTSR_MASK;
	
	//cofigure PWT(Ready & overrun Interrupt are enabled, but PWTIE is RESET
	PWT->R1 |= cpwt->Init.ClockSource | cpwt->Init.Inx | cpwt->Init.Edge | cpwt->Init.PreScaler;
	
	//Unlock Process & Msp Init
	if(cpwt->gState == CSL_PWT_STATE_RESET)
	{
		__CSL_UNLOCK(cpwt);
		CSL_PWT_MspInit(cpwt);
	}
	
	//Configure Pins
	if(cpwt->Init.Inx == PWT_IN_0)
	{
		SIM->PINSEL1 &= ~SIM_PINSEL1_PWTIN0PS_MASK;
		SIM->PINSEL1 |= cpwt->Init.Input_Pin;
	}
	else
	{
		SIM->PINSEL1 &= ~SIM_PINSEL1_PWTIN1PS_MASK;
		SIM->PINSEL1 |= cpwt->Init.Input_Pin;
	}
	
	//Configure ALTCLK
	if(cpwt->Init.ClockSource == PWT_CLKSource_TCLK)
	{
		SIM->PINSEL &= SIM_PINSEL_PWTCLKPS_MASK;
		SIM->PINSEL |= cpwt->Init.TCLK_Pin;
	}
	
	//Enable PWT
	__PWT_ENBALE();
	
	cpwt->gState = CSL_PWT_STATE_READY;
	return CSL_OK;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
CSL_StatusTypeDef CSL_PWT_DeInit(PWT_HandleTypeDef* cpwt)
{
	//Disable PWT
	__PWT_DISABLE();
	
	//Clear All Control & State Bits in PWT->R1
	PWT->R1 &= ~0xFFFF0080u;
	
	//Msp Deinit
	CSL_PWT_MspDeInit(cpwt);
	
	//Reset Handle gState
	cpwt->gState = CSL_PWT_STATE_RESET;

	return CSL_OK;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
__weak void CSL_PWT_MspInit(PWT_HandleTypeDef* cpwt)
{
	UNUSED(cpwt);
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
__weak void CSL_PWT_MspDeInit(PWT_HandleTypeDef* cpwt)
{
	UNUSED(cpwt);
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
void CSL_PWT_SoftRST(PWT_HandleTypeDef* cpwt)
{
	//Software Reset of the PWT
	PWT->R1 |= PWT_R1_PWTSR_MASK;
	
	cpwt->gState = CSL_PWT_STATE_RESET;
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
uint16_t CSL_PWT_GetPPW(void)
{
	return (uint16_t)(PWT->R1 >> 16);
}

/**
 * @brief
 * @param
 * @return
 * @note
**/
uint16_t CSL_PWT_GetNPW(void)
{
	return (uint16_t)(PWT->R2);
}

/**
 * @brief	PWT Global Interrupt Handler in CSL
**/
void CSL_PWT_IRQHandler(PWT_HandleTypeDef* cpwt)
{
	//Overrun Flag is SET
	if(((PWT->R1 & PWT_R1_POVIE_MASK) != RESET) && ((PWT->R1 & PWT_R1_PWTOV_MASK) != RESET))
	{
		//Software Flag SET
		cpwt->gState = CSL_PWT_STATE_ORIF;
		//User Callback
		CSL_PWT_OverFlowCallback(cpwt);
		
		__CSL_PWT_OV_FLAGS_CLEAR();
	}
	
	//Ready Flag is SET
	if(((PWT->R1 & PWT_R1_PRDYIE_MASK) != RESET) && ((PWT->R1 & PWT_R1_PWTRDY_MASK) != RESET))
	{
		//Software Flag SET
		cpwt->gState = CSL_PWT_STATE_RIF;
		//User Callback
		CSL_PWT_ReadyCallback(cpwt);
		
		__CSL_PWT_RDY_FLAGS_CLEAR();
	}
	
	//Reset gState
	cpwt->gState = CSL_PWT_STATE_READY;
}

/**
 * @brief	PWT Ready Callback
**/
__weak void CSL_PWT_ReadyCallback(PWT_HandleTypeDef* cpwt)
{
	UNUSED(cpwt);
}

/**
 * @brief	PWT OverFlow/Overrun Callback
**/
__weak void CSL_PWT_OverFlowCallback(PWT_HandleTypeDef* cpwt)
{
	UNUSED(cpwt);
}

//EOF

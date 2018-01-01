/**
 * Title 	Clock module in CSL for KEAZxxx(Source File)
 * License	GPLv2.0
 * Author	Stark Zhang
 * Debug	None
**/

#include "KinetisKE_csl_clk.h"

/**
 * @brief	Config the System Clock
 * @param	CLK_ConfigTypeDef *CLK_ConfigStructure
				the structure of Clock configuration
 * @return 	CSL_StatusTypeDef
 * @note	it is not supported to BYPASS & Low-power mode
**/
CSL_StatusTypeDef CSL_CLK_ClockSourceConfig(CLK_ConfigTypeDef *CLK_ConfigStructure)
{
	assert_param(IS_CLK_RDIV(CLK_ConfigStructure->RDIV));
	assert_param(IS_CLK_BDIV(CLK_ConfigStructure->BDIV));
	assert_param(IS_OSC_OS(CLK_ConfigStructure->Type));
	assert_param(IS_OSC_PowerMODE(CLK_ConfigStructure->PowerMode));
	assert_param(IS_Core_PSC(CLK_ConfigStructure->Core_PreScaler));
	assert_param(IS_Bus_PSC(CLK_ConfigStructure->Bus_PreScaler));

	//System CLock Divider Setting
	SIM->CLKDIV = CLK_ConfigStructure->Core_PreScaler | CLK_ConfigStructure->Bus_PreScaler;

	//if RDIV is out of range, return CSL_Error
	if (CLK_ConfigStructure->Range == OSC_RANGE_HF && CLK_ConfigStructure->RDIV > ICS_RDIV_DIV5)
	{
		return CSL_Error;
	}

	//Write ICS->C2
	ICS->C2 = CLK_ConfigStructure->BDIV;

	//Write ICS->C1
	ICS->C1 = CLK_ConfigStructure->SystemClkSource | CLK_ConfigStructure->RDIV | CLK_ConfigStructure->FLLClkSource;
	
	//if Engaged the External Oscillator
	if (CLK_ConfigStructure->SystemClkSource == ICS_CLKSource_ERC || (CLK_ConfigStructure->SystemClkSource == ICS_CLKSource_FLL && CLK_ConfigStructure->FLLClkSource == ICS_FLL_ERC))
	{
		//Write OSC Registers
		OSC->CR = OSC_CR_OSCEN_MASK | (uint8_t)CLK_ConfigStructure->Type | CLK_ConfigStructure->Range | CLK_ConfigStructure->PowerMode;

		while (__CSL_ICS_GetState(ICS_S_IREFST_MASK) == SET)
		{
			//Check the External Clock Source
		}
	}
	//if Engaged the Internal Oscillator
	else
	{
		//Enable IRC
		ICS->C1 |= ICS_C1_IRCLKEN_MASK;
	}

	//FBI Mode, wait for Clock Source Selection
	if(CLK_ConfigStructure->SystemClkSource == ICS_CLKSource_IRC)
	{
		while((ICS->S & ICS_S_CLKST_MASK) != 0x04u)
		{
			//wait until internal reference is selected
		}
	}

	//FBE Mode, wait for Clock Source Selection
	if(CLK_ConfigStructure->SystemClkSource == ICS_CLKSource_ERC)
	{
		while((ICS->S & ICS_S_CLKST_MASK) != 0x08u)
		{
			//wait until internal reference is selected
		}
	}

	//FEE or FEI mode, wait for FLL lock
	if(CLK_ConfigStructure->SystemClkSource == ICS_CLKSource_FLL)
	{	
		while(__CSL_ICS_GetState(ICS_S_LOCK_MASK) == RESET)
		{
			//Wait for FLL Lock
		}
	}
	
	//Msp Configure
	CSL_CLK_MspConfig(CLK_ConfigStructure);

	return CSL_OK;
}

/**
 * @brief	User Configuration
 * @param	None
 * @return 	None
 * @note	Do not modify this fucntion, user should re-define it in user's code
**/
__weak void CSL_CLK_MspConfig(CLK_ConfigTypeDef* CLK_ConfigStructure)
{
	UNUSED(0);
}

/**
 * @brief 	Enable Bus/Flash Clock Output on PTH2
 * @param	uint8_t busref
				Bus/Flash Clock Output Pre-scaler
 * @return 	None
 * @note 	None
**/
void CSL_CLK_BusClockOutputEnable(uint32_t busref)
{
	assert_param(IS_CLK_BUSREF(busref));

	//Enable Bus/Flash Clock Output
	SIM->SOPT0 |= SIM_SOPT0_CLKOE_MASK;

	//Set Pre-Scaler
	SIM->SOPT0 |= busref;
}

/**
 * @brief 	Disable Bus/Flash Clock Output on PTH2
 * @param	None
 * @return 	None
 * @note 	None
**/
void CSL_CLK_BusClockOutputDisable(void)
{
	//Disable Bus/Flash Clock Output
	SIM->SOPT0 &= ~SIM_SOPT0_CLKOE_MASK;

	//Set Pre-Scaler as 0x0u
	SIM->SOPT0 &= ~SIM_SOPT0_BUSREF_MASK;
}

/**
 * @brief 	Get Core Clock Frequency in Hz
 * @param	None
 * @return 	uint32_t
				Core Clock
 * @note 	None
**/
uint32_t CSL_CLK_GetCoreFrequency(void)
{
	return SystemCoreClock;
}

/**
 * @brief 	Get Bus Clock Frequency in Hz
 * @param	None
 * @return 	uint32_t
				Bus Clock
 * @note 	None
**/
uint32_t CSL_CLK_GetBusFrequency(void)
{
	return SystemBusClock;
}

/* Interrupt functions */

/**
 * @brief	CSL ICS IRQHandler
**/
void CSL_ICS_IRQHandler(void)
{
	//FLL Loss of Lock
	if (__CSL_ICS_GetState(ICS_S_LOLS_MASK) == SET)
	{
		//Clear ICS->S[LOLS]
		__ICS_FLL_CLEAR_FLAG();

		//ICS IRQHandler callback
		CSL_ICS_Callback();
	}
}

/**
 * @brief	CSL ICS IRQHandler User Callback
**/
__weak void CSL_ICS_Callback(void)
{
	__no_operation();
}

//EOF

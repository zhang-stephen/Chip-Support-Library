/**
 * Title 	FlexTimer module in CSL for KEAZxxx(Header File)
 * License	GPLv2.0
 * Author	Stark Zhang
 * Debug	None
**/

/* This Module supports Basic Counter, Input Capture, and Output Compare(PWM) only */

#ifndef __KinetisKE_CSL_FTM_H
#define __KinetisKE_CSL_FTM_H

#include "KinetisKE_csl.h"

/**
 * FTM Basic Initialized Structure
**/
typedef struct
{
	uint8_t ClockSource;
	uint8_t PreScaler;
	uint8_t CountMode;
	uint16_t Period;
	uint8_t Repetition;
}FTM_Base_InitTypeDef;

/**
 * Output Compare Structure
**/
typedef struct
{
	uint8_t OCMode;
	uint16_t Pulse;
}FTM_OC_InitTypeDef;

/**
 * Input Capture Structure
**/
typedef struct
{
	uint8_t ICPolarity;
}FTM_IC_InitTypeDef;

/** 
 * FTM State structures definition  
**/ 
typedef enum
{
  CSL_FTM_STATE_RESET             = 0x00U,    /*!< Peripheral not yet initialized or disabled  */
  CSL_FTM_STATE_READY             = 0x01U,    /*!< Peripheral Initialized and ready for use    */
  CSL_FTM_STATE_BUSY              = 0x02U,    /*!< An internal process is ongoing              */
  CSL_FTM_STATE_TIMEOUT           = 0x03U,    /*!< Timeout state                               */
  CSL_FTM_STATE_ERROR             = 0x04U     /*!< Reception process is ongoing                */
}CSL_FTM_StateTypeDef;

typedef struct 
{
	FTM_Type* Instance;
	FTM_Base_InitTypeDef Init;
	
	CSL_LockTypeDef Lock;
	__IO CSL_FTM_StateTypeDef gState;
}FTM_HandleTypeDef;

/**
 * FTM Channel
**/
#define FTM_CHANNEL_0			0x00u
#define FTM_CHANNEL_1			0x01u
#define FTM_CHANNEL_2			0x02u
#define FTM_CHANNEL_3			0x03u
#define FTM_CHANNEL_4			0x04u
#define FTM_CHANNEL_5			0x05u

/**
 * FTM Count Mode
**/
#define FTM_COUNTMODE_UP									0x00u
#define FTM_COUNTMODE_CENTERALIGNED							FTM_SC_CPWMS_MASK

/**
 * FTM Clock Source
**/
#define FTM_CLKSource_DISABLE								0x00u
#define FTM_CLKSource_SystemCLK								0x04u
#define FTM_CLKSource_ICSFFCLK								0x08u
#define FTM_CLKSource_TCLK									FTM_SC_CLKS_MASK

/**
 * FTM Clock Source PreScaler
**/
#define FTM_CLKSource_PSC1									0x00u
#define FTM_CLKSource_PSC2									0x01u
#define FTM_CLKSource_PSC4									0x02u
#define FTM_CLKSource_PSC8									0x03u
#define FTM_CLKSource_PSC16									0x04u
#define FTM_CLKSource_PSC32									0x05u
#define FTM_CLKSource_PSC64									0x06u
#define FTM_CLKSource_PSC128								FTM_SC_PS_MASK

/**
 * External Clock Source
**/
#define FTM0_TCLK_Ext0										((uint32_t)0x00000000u)
#define FTM0_TCLK_Ext1										((uint32_t)0x01000000u)
#define FTM0_TCLK_Ext2										((uint32_t)0x02000000u)
#define FTM1_TCLK_Ext0										((uint32_t)0x00000000u)
#define FTM1_TCLK_Ext1										((uint32_t)0x04000000u)
#define FTM1_TCLK_Ext2										((uint32_t)0x08000000u)
#define FTM2_TCLK_Ext0										((uint32_t)0x00000000u)
#define FTM2_TCLK_Ext1										((uint32_t)0x10000000u)
#define FTM2_TCLK_Ext2										((uint32_t)0x20000000u)

/**
 * FTM Output Compare & PWM Mode
**/ 
#define FTM_OC_MATCH_Toggle									0x20u				//if CnV and MOD are in a match, Toggle Channel OUTPUT
#define FTM_OC_MATCH_SET									0x24u				//if..., Channel Level is High
#define FTM_OC_MATCH_RESET									0x28u				//if..., Channel Level is low
#define FTM_OC_PWM_MODE1									0x56u				//if..., PWM level is High
#define FTM_OC_PWM_MODE2									0x60u				//if..., PWM level is low

/**
 * FTM Input Capture Polarity
**/
#define FTM_ICPOLARITY_RISIGN								0x04u
#define FTM_ICPOLARITY_FALLING								0x08u
#define FTM_ICPOLARITY_BOTHEDGE								0x0Cu

/* Macros Functions */
/**
* @brief	Enable/Disable the OverFlow Interrupt
**/
#define __CSL_FTM_TOIE_ENABLE(__HANDLE__)							(SET_BIT(__HANDLE__->Instance->SC, FTM_SC_TOIE_MASK))
#define __CSL_FTM_TOIE_DISABLE(__HANDLE__)							(CLEAR_BIT(__HANDLE__->Instance->SC, FTM_SC_TOIE_MASK))

/**
 * @brief 	Clear FTM OverFlow Interrupt Flag
**/
#define __CSL_FTM_TOIE_CLEAR_FLAG(__HANDLE__)						(CLEAR_BIT(__HANDLE__->Instance->SC, FTM_SC_TOE_MASK))

/**
 * @brief	Enable/Disable Channel Interrupt
**/
#define __CSL_FTM_CHIE_ENABLE(__HANDLE__, __CHANNEL__)				(SET_BIT(FTM_CnSC_REG(__HANDLE__, __CHANNEL__), FTM_CnSC_CHIE_MASK))
#define __CSL_FTM_CHIE_DISABLE(__HANDLE__, __CHANNEL__)				(CLEAR_BIT(FTM_CnSC_REG(__HANDLE__, __CHANNEL__), FTM_CnSC_CHIE_MASK))

/**
 * @brief	Clear Channel Interrupt Flags
**/
#define __CSL_FTM_CHIE_CLEAR_FLAG(__HANDLE__, __CHANNEL__)			{	\
																		READ_REG(FTM_CnSC_REG(__HANDLE__, __CHANNEL__)); \
																		CLEAR_BIT(FTM_CnSC_REG(__HANDLE__, __CHANNEL__), FTM_CnSC_CHF_MASK); \
																	}

/**
 * @brief 	Enable/Disable FTM Fault Interrupt(only for FTM2)
**/
#define __CSL_FTM_FAULTIE_ENABLE(__HANDLE__)						(SET_BIT(FTM_MODE_REG(FTM2, FTM_MODE_FAULTIE_MASK))
#define __CSL_FTM_FAULTIE_DISABLE(__HANDLE__)						(CLEAR_BIT(FTM_MODE_REG(FTM2, FTM_MODE_FAULTIE_MASK))

/**
 * @brief	Clear Fault Interrupt Interrupt Flag
**/
#define __CSL_FTM_FAULTIE_CLEAR_FLAG(__HANDLE__)					{	\
																		READ_REG(FTM2->FMS); \
																		CLEAR_BIT(FTM2->FMS, FTM_FMS_FAULTF_MASK); \
																	}

/**
 * @brief	Enable/Disable Write Protection
**/
#define __CSL_FTM_WP_DISABLE(__HANDLE__)							{	\
																		READ_REG(__HANDLE__->FMS); \
																		SET_BIT(__HANDLE__->MODE, FTM_MODE_WPDIS_MASK); \
																	}
#define __CSL_FTM_WP_ENABLE(__HANDLE__)								SET_BIT(__HANDLE__->FMS, FTM_FMS_WPEN_MASK);

/**
 * @brief 	Get Channel Value 
**/
#define __CSL_FTM_GET_CHANNEL_VAL(__HANDLE__, __CHANNEL__)			(READ_REG(FTM_CnV_REG(__HANDLE__, __CHANNEL__)))

/**
 * @brief	Set Channel Value in OC/PWM mode
**/
#define __CSL_FTM_SET_CHANNEL_VAL(__HANDLE__, __CHANNEL__, VAL)		(FTM_CnV_REG(__HANDLE__, __CHANNEL__) = VAL)

/**
 * @brief 	Reset FTM Counter
**/
#define __CSL_FTM_COUNT_RST(__HANDLE__)								(__HANDLE__->Instance->CNT = 0xFFFFu)

/* Public Functions of FTM */
//Basic Functions 
CSL_StatusTypeDef CSL_FTM_Base_Init(FTM_HandleTypeDef* cftm);
CSL_StatusTypeDef CSL_FTM_Base_DeInit(FTM_HandleTypeDef* cftm);
void CSL_FTM_Base_MspInit(FTM_HandleTypeDef* cftm);
void CSL_FTM_Base_MspDeInit(FTM_HandleTypeDef* cftm);

CSL_StatusTypeDef CSL_FTM_Base_Start(FTM_HandleTypeDef* cftm);
CSL_StatusTypeDef CSL_FTM_Base_Stop(FTM_HandleTypeDef* cftm);
CSL_StatusTypeDef CSL_FTM_Base_Start_IT(FTM_HandleTypeDef* cftm);
CSL_StatusTypeDef CSL_FTM_Base_Stop_IT(FTM_HandleTypeDef* cftm);

//OutPut Compare
CSL_StatusTypeDef CSL_FTM_OC_Init(FTM_HandleTypeDef* cftm);
CSL_StatusTypeDef CSL_FTM_OC_DeInit(FTM_HandleTypeDef* cftm);
void CSL_FTM_OC_MspInit(FTM_HandleTypeDef* cftm);
void CSL_FTM_OC_MspDeInit(FTM_HandleTypeDef* cftm);

CSL_StatusTypeDef CSL_FTM_OC_Start(FTM_HandleTypeDef* cftm);
CSL_StatusTypeDef CSL_FTM_OC_Stop(FTM_HandleTypeDef* cftm);
CSL_StatusTypeDef CSL_FTM_OC_Start_IT(FTM_HandleTypeDef* cftm);
CSL_StatusTypeDef CSL_FTM_OC_Stop_IT(FTM_HandleTypeDef* cftm);

//PWM Mode
CSL_StatusTypeDef CSL_FTM_PWM_Init(FTM_HandleTypeDef* cftm);
CSL_StatusTypeDef CSL_FTM_PWM_DeInit(FTM_HandleTypeDef* cftm);
void CSL_FTM_PWM_MspInit(FTM_HandleTypeDef* cftm);
void CSL_FTM_PWM_MspDeInit(FTM_HandleTypeDef* cftm);

CSL_StatusTypeDef CSL_FTM_PWM_Start(FTM_HandleTypeDef* cftm);
CSL_StatusTypeDef CSL_FTM_PWM_Stop(FTM_HandleTypeDef* cftm);
CSL_StatusTypeDef CSL_FTM_PWM_Start_IT(FTM_HandleTypeDef* cftm);
CSL_StatusTypeDef CSL_FTM_PWM_Stop_IT(FTM_HandleTypeDef* cftm);

//Input Capture
CSL_StatusTypeDef CSL_FTM_IC_Init(FTM_HandleTypeDef* cftm);
CSL_StatusTypeDef CSL_FTM_IC_DeInit(FTM_HandleTypeDef* cftm);
void CSL_FTM_IC_MspInit(FTM_HandleTypeDef* cftm);
void CSL_FTM_IC_MspDeInit(FTM_HandleTypeDef* cftm);

CSL_StatusTypeDef CSL_FTM_IC_Start(FTM_HandleTypeDef* cftm);
CSL_StatusTypeDef CSL_FTM_IC_Stop(FTM_HandleTypeDef* cftm);
CSL_StatusTypeDef CSL_FTM_IC_Start_IT(FTM_HandleTypeDef* cftm);
CSL_StatusTypeDef CSL_FTM_IC_Stop_IT(FTM_HandleTypeDef* cftm);

//Interrupt Functions
void CSL_FTM_IRQHandler(FTM_HandleTypeDef* cftm);
void CSL_FTM_ChannelEventCallback(FTM_HandleTypeDef* cftm);
void CSL_FTM_PeriodElapsedCallback(FTM_HandleTypeDef* cftm);
void CSL_FTM_FaultInCallback(FTM_HandleTypeDef* cftm);

//Peripherals Functions
CSL_StatusTypeDef CSL_FTM_OC_Config(FTM_HandleTypeDef* cftm, FTM_OC_InitTypeDef* sConfig, uint8_t Channel);
CSL_StatusTypeDef CSL_FTM_PWM_Config(FTM_HandleTypeDef* cftm, FTM_OC_InitTypeDef* sConfig, uint8_t Channel);
CSL_StatusTypeDef CSL_FTM_IC_Config(FTM_HandleTypeDef* cftm, FTM_IC_InitTypeDef* sConfig, uint8_t Channel);

//Handle Functions
CSL_FTM_StateTypeDef CSL_FTM_GetState(FTM_HandleTypeDef* cftm);

/* Defgroup for FTM Module Parameters Check */
#define IS_FTM_CLKSource(clk)							((clk == FTM_CLKSource_DISABLE) || \
														 (clk == FTM_CLKSource_SystemCLK) || \
														 (clk == FTM_CLKSource_ICSFFCLK) || \
														 (clk == FTM_CLKSource_TCLK))

#define IS_FTM_PreScaler(psc)							((psc == FTM_CLKSource_PSC1) || \
														 (psc == FTM_CLKSource_PSC2) || \
														 (psc == FTM_CLKSource_PSC4) || \
														 (psc == FTM_CLKSource_PSC8) || \
														 (psc == FTM_CLKSource_PSC16) || \
														 (psc == FTM_CLKSource_PSC32) || \
														 (psc == FTM_CLKSource_PSC64) || \
														 (psc == FTM_CLKSource_PSC128))

#define IS_FTM_CHANNEL(ch)								((ch == FTM_CHANNEL_0) || \
														 (ch == FTM_CHANNEL_1) || \
														 (ch == FTM_CHANNEL_2) || \
														 (ch == FTM_CHANNEL_3) || \
														 (ch == FTM_CHANNEL_4) || \
														 (ch == FTM_CHANNEL_5))

#define IS_FTM_OCMode(oc)								((oc == FTM_OC_MATCH_Toggle) || \
														 (oc == FTM_OC_MATCH_SET) || \
														 (oc == FTM_OC_MATCH_RESET) || \
														 (oc == FTM_OC_PWM_MODE1) || \
														 (oc == FTM_OC_PWM_MODE2))

#define IS_FTM_ICPolarity(polar)						((polar == FTM_ICPOLARITY_RISIGN) || \
														 (polar == FTM_ICPOLARITY_FALLING) || \
														 (polar == FTM_ICPOLARITY_BOTHEDGE))

#endif /*__KinetisKE_CSL_FTM_H*/

//EOF

/**
 * Title 	Watch Dog module in CSL for KEAZ128(Header File)
 * License	GPLv2.0
 * Author	Stark Zhang
 * Debug	None
**/

#ifndef __KinetisKE_CSL_WDOG_H
#define __KinetisKE_CSL_WDOG_H

#include "KinetisKE_csl.h"

typedef struct
{
    uint8_t Mode;
    uint8_t PreScaler;
    uint8_t ClockSource;
    uint16_t CountValue;
    uint16_t WindowValue;
}WDOG_HandleTypeDef;

/**
 * WDOG Mode
**/
#define WDOG_MODE_NORMAL                    0x00u
#define WDOG_MODE_WINDOW                    WDOG_CS2_WIN_MASK

/**
 * WDOG ClockSource PreScaler
**/
#define WDOG_CLK_PSC1                       0x00u
#define WDOG_CLK_PSC256                     WDOG_CS2_PRES_MASK

/**
 * WDOG Clock Source
**/
#define WDOG_CLKSource_Bus                  0x00u
#define WDOG_CLKSource_LPO                  0x01u
#define WDOG_CLKSource_IRC                  0x02u
#define WDOG_CLKSource_ERC                  WDOG_CS2_CLK_MASK

/* Macros Functions for Watch Dog */
/**
 * @brief   Enable/Disable the WDOG Interrupt
**/
#define __CSL_WDOG_IT_ENABLE()              (SET_BIT(WDOG->CS1, WDOG_CS1_INT_MASK))
#define __CSL_WDOG_IT_DISABLE()             (CLEAR_BIT(WDOG->CS1, WDOG_CS1_INT_MASK))

/**
 * @brief   Clear WDOG Interrupt Flag
**/
#define __CSL_WDOG_CLEAR_FLAG()             (SET_BIT(WDOG->CS2, WDOG_CS2_FLG_MASK))

/**
 * @brief   Allow to Update/re-configure the WDOG
**/
#define __CSL_WDOG_ALLOW_UPDATE()           (SET_BIT(WDOG->CS1, WDOG_CS1_UPDATE_MASK))

/**
 * @brief   Get WDOG Counter Value
**/
#define __CSL_WDOG_GET_COUNT()              (READ_REG(WDOG->CNT))

/**
 * @brief   Get WDOG WindowValue
**/
#define __CSL_WDOG_GET_WINVAL()             (READ_REG(WDOG->WIN))

/**
 * @brief 	Unlock the WDOG
**/
#define __CSL_WDOG_UNLOCK()					{									 \
												__CSL_GIRQ_DISABLE();			 \
                                                WRITE_REG(WDOG->CNT, 0x20C5u);   \
                                                WRITE_REG(WDOG->CNT, 0x28D9u);   \
												__CSL_GIRQ_ENABLE();			 \
                                            }

/**
 * @brief   Refresh WDOG Counter
**/  
#define __CSL_WODG_REFRESH()                {                                    \
                                                WRITE_REG(WDOG->CNT, 0x02A6u);   \
                                                WRITE_REG(WDOG->CNT, 0x80B4u);   \
                                            }

/* Functions for Watch Dog Module */
CSL_StatusTypeDef CSL_WDOG_Config(WDOG_HandleTypeDef* cwdog);
void CSL_WDOG_MspConfig(WDOG_HandleTypeDef* cwdog);

void CSL_WDOG_Refresh(WDOG_HandleTypeDef* cwdog);

void CSL_WDOG_IRQHandler(WDOG_HandleTypeDef* cwdog);
void CSL_WDOG_Callback(WDOG_HandleTypeDef* cwdog);

#endif /*__KinetisKE_CSL_WDOG_H*/

//EOF
